#include "QuadTree.hpp"

#include <iostream>
#include <fstream>

#include <string>
#include <vector>

#include <ctime> 
#include <ratio>
#include <chrono> 
#include <iomanip>

using std::cout, std::endl, std::string, std::vector;

// Compile: g++ -Wall -std=c++17 -o readDataSet tests/readDataSet.cpp source/*.cpp -I include

struct CityInfo {
    string name;
    int population;
    double lat, lon;
    
    CityInfo(string n, int p, double la, double lo) 
    : name(n), population(p), lat(la), lon(lo) {};
};


vector<CityInfo> readDataSet(int nCities, string dataSetPath) {
    vector<CityInfo> cities;
    
    std::ifstream dataSet(dataSetPath);
    if (!dataSet.is_open()) return cities;

    int countLine = 0;
    bool firstLine = true;
    while (dataSet) {
        string line;
        getline(dataSet, line);
        
        // Saltar primera linea
        if (firstLine) {
            firstLine = false;
            continue;
        }

        // Extraer información de la linea
        string name;
        int population;
        double latitude, longitude;
        int countColon = 0;
        for (size_t i = 0; i < line.size(); i++) {  
            if (countColon == 1 && line[i] != ';') {
                name += line[i];
            } 
            if (countColon == 4) {
                int init = i;
                while (line[i] != ';') i++;
                population = stoi(line.substr(init, i-init));
            }
            if (countColon == 7) {
                int init = i;
                while (line[i] != ',') i++;
                latitude = stod(line.substr(init, i-init));

                init = i+1;
                while (i < line.size()) i++;
                longitude = stod(line.substr(init, i-init));
            }

            if (line[i] == ';') countColon++;
        }

        // Guardar información en el vector
        cities.push_back(CityInfo(name, population, latitude, longitude));

        countLine++;
        if (countLine == nCities) break;
    }

    dataSet.close();
    return cities;
}

bool fileExists(string fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

void updateInsertResults(int nCities, double operationTime) {
    if (!fileExists("insertResults.csv")) {
        std::ofstream results("insertResults.csv");
        results << "CantidadCiudades;TiempoPromedio(ms)" << endl;
        results.close();
    }

    std::ofstream results("insertResults.csv", std::ios_base::app);

    results << nCities << ";" << operationTime << endl;
    
    results.close();
}

void updateCountRegionFile(int nCities, double operationTime) {
    if (!fileExists("countRegionResults.csv")) {
        std::ofstream results("countRegionResults.csv");
        results << "CantidadCiudades;TiempoPromedio(ms)" << endl;
        results.close();
    }

    std::ofstream results("countRegionResults.csv", std::ios_base::app);

    results << nCities << ";" << operationTime << endl;
    
    results.close();
}

void updateAggregationRegionFile(int nCities, double operationTime) {
    if (!fileExists("agreggationRegionResults.csv")) {
        std::ofstream results("agreggationRegionResults.csv");
        results << "CantidadCiudades;TiempoPromedio(ms)" << endl;
        results.close();
    }
    
    std::ofstream results("agreggationRegionResults.csv", std::ios_base::app);

    results << nCities << ";" << operationTime << endl;
    
    results.close();
}

void printCities(vector<CityInfo> cities) {
    cout << "Cities: " << cities.size() << endl;
    for (size_t i = 0; i < cities.size(); i++) {
        cout << cities[i].name << " " 
             << cities[i].population << " " 
             << cities[i].lat << " " 
             << cities[i].lon << endl;
    }
}

int main() {
    srand(time(NULL));
    
    vector<CityInfo> cities = readDataSet(1600000, "worldcitiespop_fixed.csv");
    // printCities(cities);

    int reps = 20;
    double duration = 0;
    double fduration = 0;
    
    for (int k = 0; k < 5; k++) {
        int nCities = 100000*(int)pow(2,k);
        duration = 0;
        fduration = 0;

        for (int j = 0; j < reps; j++){
            QuadTree* qt = new QuadTree(Point(-100.0, -100.0), Point(100.0, 100.0));

            for (int i = 0; i < nCities; i++) {
                auto start = std::chrono::high_resolution_clock::now();
                
                qt->insert(Point(cities[i].lat, cities[i].lon), cities[i].population, cities[i].name);
                    
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end - start;
                duration += diff.count();
            }

            duration /= cities.size();
            fduration += duration;
            delete qt;
        }
        fduration /= reps;
        cout << "Tiempo: " << fduration << endl;
        updateInsertResults(nCities, fduration);
    }
    
    /*
    for (int k = 0; k < 5; k++) {
        QuadTree* qt = new QuadTree(Point(-100.0, -100.0), Point(100.0, 100.0));
        size_t nCities = 100000*(int)pow(2,k);

        for (size_t i = 0; i < nCities; i++) {
            qt->insert(Point(cities[i].lat, cities[i].lon), cities[i].population, cities[i].name);
        }

        duration = 0;
        fduration = 0;
        for (int j = 0; j < reps; j++){
            for (size_t i = 1; i < 6; i++) {
                auto start = std::chrono::high_resolution_clock::now();
                qt->countRegion(Point(0,0),(int)pow(2,(3*i)+2));
                    
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end - start;
                duration += diff.count();
            }

            duration /= 5;
            fduration += duration;
        }
        fduration /= reps;
        cout << "Tiempo CountRegion: " << fduration << endl;
        updateCountRegionFile(nCities, fduration);

        duration = 0;
        fduration = 0;
        for (int j = 0; j < reps; j++){
            for (size_t i = 1; i < 6; i++) {
                auto start = std::chrono::high_resolution_clock::now();
                
                qt->aggregateRegion(Point(0,0),(int)pow(2,(3*i)+2));
                    
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end - start;
                duration += diff.count();
            }

            duration /= 5;
            fduration += duration;
        }
        fduration /= reps;
        cout << "Tiempo AggregationRegion: " << fduration << endl;
        updateAggregationRegionFile(nCities, fduration);
        delete qt;
    }
    */
   
    return 0;
}
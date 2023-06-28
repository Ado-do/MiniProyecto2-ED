#include "TestUtils.hpp"

using std::cout, std::endl, std::string, std::vector;

CityInfo::CityInfo(string n, int p, double la, double lo) : name(n), population(p), lat(la), lon(lo) {}

void CityInfo::print() {
    cout << "City: " << name << 
           " Population: " << population << 
           " Lat: " << lat << 
           " Lon: " << lon << endl;
}


vector<CityInfo> TestUtils::readDataSet(string dataSetPath) {
    int nCities = 1600000;
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

void TestUtils::updateCVSFile(string fileName, int nCities, double operationTime) {
    if (!fileExists(fileName)) {
        std::ofstream results(fileName);
        results << "CantidadCiudades;TiempoPromedio(ms)" << endl;
        results.close();
    }

    std::ofstream results(fileName, std::ios_base::app);

    results << nCities << ";" << operationTime << endl;
    
    results.close();
}

bool TestUtils::fileExists(string fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}
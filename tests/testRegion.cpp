#include "QuadTree.hpp"
#include "TestUtils.hpp"

#include <chrono>
#include <iostream>

using std::cout, std::endl, std::vector;

// * Compile:
// g++ -Wall -std=c++17 -o build/testRegion tests/testRegion.cpp source/*.cpp -I include
// * Execute:
// ./build/testRegion


int main() {
    srand(time(NULL));
    
    vector<CityInfo> cities = TestUtils::readDataSet("data\\worldcitiespop_fixed.csv");

    // * Imprimir ciudades
    // cout << "Cities: " << cities.size() << endl;
    // for (size_t i = 0; i < 1000; i++) {
    //     cities[i].print();
    // }

    // * Insertar ciudades y medir tiempo
    int reps = 20;
    double duration = 0, fduration = 0;
    for (int k = 0; k < 5; k++) {
        QuadTree* qt = new QuadTree(Point(-100.0, -100.0), Point(100.0, 100.0));
        size_t nCities = 100000 * int(pow(2,k));

        for (size_t i = 0; i < nCities; i++) {
            qt->insert(Point(cities[i].lat, cities[i].lon), cities[i].population, cities[i].name);
        }

        duration = fduration = 0;
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
        
        TestUtils::updateCVSFile("data\\countRegion.csv", nCities, fduration);

        duration = fduration = 0;
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

        TestUtils::updateCVSFile("data\\aggregationRegion.csv", nCities, fduration);
        
        delete qt;
    }

    return 0;
}
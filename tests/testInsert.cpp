#include "QuadTree.hpp"
#include "TestUtils.hpp"

#include <chrono>
#include <iostream>

using std::cout, std::endl, std::vector;

// * Compile:
// g++ -Wall -std=c++17 -o build/testInsert tests/testInsert.cpp source/*.cpp -I include
// * Execute:
// ./build/testInsert


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
        int nCities = 100000 * int(pow(2,k));
        duration = fduration = 0;

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

        TestUtils::updateCVSFile("insertResults.csv", nCities, fduration);
    }

    return 0;
}
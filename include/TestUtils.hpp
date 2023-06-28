#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct CityInfo {
    std::string name;
    int population;
    double lat, lon;
    
    CityInfo(std::string, int, double, double);
    void print();
};

namespace TestUtils {
    std::vector<CityInfo> readDataSet(std::string);
    void updateCVSFile(std::string, int, double);
    bool fileExists(std::string);
}

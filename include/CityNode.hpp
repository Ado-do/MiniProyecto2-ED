#pragma once

#include "Point.hpp"

#include <string>

struct CityNode {
    Point point;
    int data;
    std::string city;

    CityNode(Point = Point(), int = -1, std::string = "N/A");
};

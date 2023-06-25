#pragma once

#include "Point.hpp"

Point::Point(){
    x = -1;
    y = -1;
}
Point::Point(int u, int v){
    x = u;
    y = v;
}
void Point::setPoint(int u, int v){
    x = u;
    y = v;
}
int Point::getX(){
    return(x);
}
int Point::gety(){
    return(y);
}

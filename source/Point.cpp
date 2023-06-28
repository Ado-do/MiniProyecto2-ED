#include "Point.hpp"

Point::Point(double u, double v) : dx(u), dy(v) {
    x = (int)(u*1000);
    y = (int)(v*1000);
 }
Point::Point(int u, int v) : x(u), y(v) { }

void Point::setPoint(double u, double v) {
    dx = u;
    dy = v;
    x = (int)(u*1000);
    y = (int)(v*1000);
}

void Point::setPoint(int u, int v) {
    x = u;
    y = v;
}

int Point::getX() const { return x; }

int Point::getY() const { return y; }

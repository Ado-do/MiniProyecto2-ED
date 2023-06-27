#include "..\include\Point.hpp"

Point::Point(int u, int v) : x(u), y(v) { }

void Point::setPoint(int u, int v) {
    x = u;
    y = v;
}

int Point::getX() { return x; }

int Point::getY() { return y; }

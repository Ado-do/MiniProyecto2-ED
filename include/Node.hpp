#pragma once

#include "Point.hpp"

class Node {
  private:
    Point point;
    int data;

  public:
    Node();
    Node(Point point, int data);
    ~Node();
    void setdata(int data);
    void setPoint(Point point);
    int getdata();
    Point getPoint();
};
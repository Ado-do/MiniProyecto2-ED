#pragma once

#include "Point.hpp"

enum NodeType { WHITE, BLACK };

class Node {
  private:
    Point* point;
    int data;
    NodeType type;

    Node* NW;
    Node* NE;
    Node* SW;
    Node* SE;

  public:
    Node();
    ~Node();
    void setNW(Node* NewNode);
    void setNE(Node* NewNode);
    void setSW(Node* NewNode);
    void setSE(Node* NewNode);
    void setdata(int data);
    void stype();
    void setPoint(Point* point);
    Node* getNW();
    Node* getNE();
    Node* getSW();
    Node* getSE();
    int getdata();
    NodeType gettype();
    Point* getPoint();
};
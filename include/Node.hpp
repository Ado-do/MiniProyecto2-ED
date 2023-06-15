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
};
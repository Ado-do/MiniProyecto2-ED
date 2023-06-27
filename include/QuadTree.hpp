#pragma once

#include "Point.hpp"

#include <cmath>
#include <vector>
#include <iostream>

enum RegionType { WHITE, BLACK };

struct Node {
    Point point;
    int data;

    Node(Point = Point(), int = -1);
};

class QuadTree {
  private:
    Node* node = nullptr;

    Point topLeftPoint;
    Point originalBotRightPoint; // Limite Original
    Point botRightPoint; // Limite Modificado

    QuadTree* topLeftQT = nullptr;
    QuadTree* topRightQT = nullptr;
    QuadTree* botLeftQT = nullptr;
    QuadTree* botRightQT = nullptr;

    RegionType type;

    int count;
    int dataSum;
    int nodesCount;

    // * Funciones privadas las Cuales Buscamos que el usuario no use y sirvan como ayuda para otras funciones
    int countRegionAux(Point pTL, Point pBR, QuadTree* QT);
    int aggregateRegionAux(Point pTL, Point pBR, QuadTree* QT);
    void dataSumChange(Point, int, int);

  public:
    QuadTree(Point, Point, bool = true);
    ~QuadTree();

    // * Extras
    RegionType getType();
    bool inBounds(Point p);
    Node* search(Point);

    // * Por enunciado
    int totalPoints();
    int totalNodes();
    void insert(Point p, int data);
    std::vector<Node*> list();
    int countRegion(Point p, int d);
    int aggregateRegion(Point p, int d);
};

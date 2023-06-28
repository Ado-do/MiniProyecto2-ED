#pragma once

#include "Point.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

enum RegionType { WHITE, BLACK };

struct Node {
    Point point;
    int data;
    std::string city;

    Node(Point = Point(), int = -1, std::string = "N/A");
};

class QuadTree {
  private:
    std::vector<Node*> nodes; // * Vector de Nodos

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

    bool iequals(const std::string& a, const std::string& b); // Comparar strings sin importar mayusculas o minusculas

   public:
    QuadTree(Point, Point, bool = true);
    ~QuadTree();

    // * Extras
    RegionType getType();
    bool inBounds(const Point);
    std::vector<Node*> search(Point);

    // * Por enunciado
    int totalPoints();
    int totalNodes();
    void insert(Point , int, std::string);
    std::vector<Node*> list();
    int countRegion(Point , int);
    int aggregateRegion(Point , int);
};

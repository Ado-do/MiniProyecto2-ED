#pragma once

#include "Point.hpp"
#include "CityNode.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

enum RegionType { WHITE, BLACK };

class QuadTree {
  private:
    std::vector<CityNode*> nodes; // * Vector de Nodos

    Point topL_point;
    Point botR_point; // Limite Modificado
    Point originalBotRPoint; // Limite Original

    QuadTree* topL_QT = nullptr;
    QuadTree* topR_QT = nullptr;
    QuadTree* botL_QT = nullptr;
    QuadTree* botR_QT = nullptr;

    RegionType type;

    int count;
    int dataSum;

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
    std::vector<CityNode*> search(Point);

    // * Por enunciado
    int totalPoints();
    int totalNodes();
    void insert(Point , int, std::string);
    std::vector<CityNode*> list();
    int countRegion(Point , int);
    int aggregateRegion(Point , int);
};

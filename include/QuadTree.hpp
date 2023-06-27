#pragma once

#include "Point.hpp"
#include "Node.hpp"

enum RegionType { WHITE, BLACK };

class QuadTree {
  private:
    Node* node;

    Point TopLeftPoint;
    Point OriginalBottomRightPoint; // Limite Original
    Point BottomRightPoint; // Limite Modificado

    QuadTree* TopLeft;
    QuadTree* TopRight;
    QuadTree* BottomLeft;
    QuadTree* BottomRight;

    RegionType type;

    int count;
    int datasum;
    int nodesCount;

    // * Funciones privadas las Cuales Buscamos que el usuario no use y sirvan como ayuda para otras funciones
    QuadTree(Point TopLeft, Point BotRight, bool t);
    int totalNodesAux(QuadTree* QT);
    int countRegionAux(Point pTL, Point pBR, QuadTree* QT);
    int AggregateRegionAux(Point pTL, Point pBR, QuadTree* QT);
    Point getPointTL();
    Point getPointBR();

  public:
    QuadTree(); // lo considero inecesario
    QuadTree(Point TopLeft, Point BotRight);
    ~QuadTree();

    // * Extras
    void clear();
    // void insert(Entity* entity);
    // void retrieve(std::vector<Entity*>& entities, Entity* entity);
    RegionType getType();
    bool inBounds(Point p);

    // * Por enunciado
    int totalPoints();
    int totalNodes();
    void insert(Point p, int data);
    // list();
    int countRegion(Point p, int d);
    int AggregateRegion(Point p, int d);
};

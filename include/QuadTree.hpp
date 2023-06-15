#pragma once

#include "Point.hpp"
#include "Node.hpp"

class QuadTree {
  private:
    Node* root;

  public:
    QuadTree();
    ~QuadTree();

    // * Extras
    void clear();
    // void insert(Entity* entity);
    // void retrieve(std::vector<Entity*>& entities, Entity* entity);

    // * Por enunciado
    int totalPoints();
    int totalNodes();
    void insert(Point p, int data);
    // list();
    int countRegion(Point p, int d);
    int AggregateRegion(Point p, int d);
};

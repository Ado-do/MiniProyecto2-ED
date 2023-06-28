#include "include\QuadTree.hpp"

#include <iostream>
#include <vector>

using std::cout, std::endl, std::vector;

// Compile: g++ -Wall -std=c++17 -o main main.cpp source/*.cpp

int main() {
    QuadTree qt(Point(0, 0), Point(8, 8));

    qt.insert(Point(0, 0), 5, "A");
    qt.insert(Point(0, 1), 4, "B");
    qt.insert(Point(1, 0), 2, "C");
    qt.insert(Point(1, 1), 2, "D");
    qt.insert(Point(2, 1), 1, "E");
    qt.insert(Point(5, 1), 3, "F");
    qt.insert(Point(6, 2), 10,"G");
    qt.insert(Point(7, 7), 8, "H");
    
    cout << "Contenido en la coordenada (5,1): ";
    vector<Node*> p = qt.search(Point(5, 1));
    if (!p.empty()) cout << p[0]->data << endl;
    else              cout << "No existe el punto." << endl;
    // Contenido en la coordenada (5,1): 3

    cout << "Contenido en la coordenada (1,5): ";
    p = qt.search(Point(1, 5));
    if (!p.empty()) cout << p[0]->data << endl;
    else              cout << "No existe el punto." << endl;
    // Contenido en la coordenada (1,5): No existe el punto.
    
    cout << "Suma de datos en region: " << qt.aggregateRegion(Point(1,1), 1) << endl;
    cout << "Cantidad de puntos en region: " << qt.countRegion(Point(0,0), 8) << endl; 

    cout << "Cantidad total de puntos: " << qt.totalPoints() << endl;
    cout << "Cantidad total de nodos: " << qt.totalNodes() << endl;

    vector<Node*> v = qt.list();
    
    // Imprimimos los puntos
    for (size_t i = 0; i < v.size(); i++) {
        cout << "Name: " << v[i]->city << " -> ";
        cout << "(" << v[i]->point.getX() << ", " << v[i]->point.getY() << ") -> " << v[i]->data << endl;
    }

    return 0;
}

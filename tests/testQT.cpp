#include "QuadTree.hpp"

#include <iostream>
#include <vector>
#include <fstream>
using std::cout, std::endl, std::vector;

// * Compile: 
// g++ -Wall -std=c++17 -o build/testQT tests/testQT.cpp source/*.cpp -I include
// * Execute: 
// ./build/testQT

int main() {
    QuadTree qt(Point(-0.008, -0.008), Point(0.000, 0.000));

    qt.insert(Point(0.000, 0.000), 5, "A");
    qt.insert(Point(0.000, -0.001), 4, "B");
    qt.insert(Point(-0.001, 0.000), 2, "C");
    qt.insert(Point(-0.001, -0.001), 2, "D");
    qt.insert(Point(-0.002, -0.001), 1, "E");
    qt.insert(Point(-0.005, -0.001), 3, "F");
    qt.insert(Point(-0.006, -0.002), 10,"G");
    qt.insert(Point(-0.007, -0.007), 8, "H");
    
    cout << "Contenido en la coordenada (5,1): ";
    vector<CityNode*> p = qt.search(Point(-5, -1));
    if (!p.empty()) cout << p[0]->data << endl;
    else            cout << "No existe el punto." << endl;
    // Contenido en la coordenada (5,1): 3

    cout << "Contenido en la coordenada (1,5): ";
    p = qt.search(Point(-1, -5));
    if (!p.empty()) cout << p[0]->data << endl;
    else            cout << "No existe el punto." << endl;
    // Contenido en la coordenada (1,5): No existe el punto.
    
    cout << "Suma de datos en region: " << qt.aggregateRegion(Point(-1,-1), 1) << endl;
    cout << "Cantidad de puntos en region: " << qt.countRegion(Point(0,0), 8) << endl; 

    cout << "Cantidad total de puntos: " << qt.totalPoints() << endl;
    cout << "Cantidad total de nodos: " << qt.totalNodes() << endl;

    vector<CityNode*> v = qt.list();
    
    // Imprimimos los puntos
    for (size_t i = 0; i < v.size(); i++) {
        cout << "Name: " << v[i]->city << " -> ";
        cout << "(" << v[i]->point.getX() << ", " << v[i]->point.getY() << ") -> " << v[i]->data << endl;
    }

    return 0;
}

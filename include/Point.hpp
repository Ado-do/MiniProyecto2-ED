#pragma once

class Point {
  private:
    int x;
    int y;
    
  public:
    Point();
    Point(int u, int v);

    void setPoint(int u, int v);
    int getX();
    int gety();
};

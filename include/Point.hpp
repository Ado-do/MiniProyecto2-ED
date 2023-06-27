#pragma once

class Point {
  private:
    int x, y;
    
  public:
    Point(int = -1, int = -1);

    void setPoint(int, int);
    int getX();
    int getY();
};

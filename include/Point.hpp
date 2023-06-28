#pragma once

class Point {
  private:
    int x, y;
    double dx,dy;
    
  public:
    Point(int = -1, int = -1); //double double x= (int)(dx*1000) y = (int)(dy*1000)

    void setPoint(int, int);
    int getX();
    int getY();
};

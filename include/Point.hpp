#pragma once

class Point {
  private:
    double dx,dy;
    int x, y;
    
  public:
    Point(double = -1, double = -1); //double double x= (int)(dx*1000) y = (int)(dy*1000)
    Point(int, int);

    void setPoint(double, double);
    void setPoint(int, int);
    int getX() const;
    int getY() const;
};

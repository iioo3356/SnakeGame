//Point.h

#ifndef _POINT_H
#define _POINT_H

#include <iostream>
#include <vector>
class Point {
public:
  int row;
  int col;
  Point(int num1=0, int num2=0);
  Point(const Point& pt);

  Point& operator=(const Point& pt);
  bool operator==(const Point& pt) const;
  bool operator!=(const Point& pt) const;
  bool isInclude(int len, const std::vector<Point> pt) const;
  friend std::ostream& operator<<(std::ostream &os, const Point& pt);
};
#endif

//Point.cpp
#include "Point.h"
using namespace std;

Point::Point(int num1, int num2): row(num1),col(num2){
  // row=num1;
  // col=num2;
}
Point::Point(const Point& pt){
  row=pt.row;
  col=pt.col;
}

Point& Point::operator=(const Point& pt){
  this->row = pt.row;
  this->col = pt.col;
  return *this;
}

bool Point::operator==(const Point& pt) const{
  return (row==pt.row)&&(col==pt.col);
}
bool Point::operator!=(const Point& pt) const{
  return !(*this==pt);
}
bool Point::isInclude(int len, const vector<Point> pt) const{
 for (int i=0;i<len;i++){
   if (*this == pt[i]){
     return true;
   }
 }
 return false;
}
ostream& operator<<(ostream &os, const Point& pt){
  os << "(" << pt.row << "," << pt.col << ")";
  return os;
}

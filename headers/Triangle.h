#ifndef TRIANGLE
#define TRIANGLE

#include "master.h"
#include "IO.h"
#include "aux.h"

class Triangle
{
public:
  Mat vertices; // 3 x 2
  Triangle(const vector<Point2d>& v);
  inline Triangle(const Mat& v) : vertices(v) {};
  inline ~Triangle(){};

  inline Point2d get(int i) const { return Point2d(vertices.at<double>(i,0), vertices.at<double>(i,1)); }
  void boundary(double& minX, double& minY, double& maxX, double& maxY) const;
  double minX() const;
  double minY() const;
  double maxX() const;
  double maxY() const;

  vector<Point2f> toFloatVector() const;
  vector<Point2d> toVector() const;
  void toIntArray(Point* p) const;
  Rect boundingRect() const;

  Triangle operator >>(const Point2d &displacement) const;
  Triangle operator <<(const Point2d &displacement) const;
};

#endif
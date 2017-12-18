#ifndef TRIANGLE
#define TRIANGLE

#include "master.h"
#include "IO.h"
#include "aux.h"

// TAOTODO: Should also hold "indices" for easier reference
class Triangle
{
public:
  Mat vertices; // 3 x 2 : vertex coordinates
  Mat ids; // 3 x 1 : vertex ids

  Triangle(const vector<Point2d>& v, int a=-1, int b=-1, int c=-1);
  Triangle(const Mat& v, const Mat& id);
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
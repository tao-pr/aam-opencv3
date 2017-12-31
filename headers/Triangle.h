#ifndef TRIANGLE
#define TRIANGLE

#include "master.h"
#include "IO.h"
#include "aux.h"

/**
 * [Triangle] class only stores the vertex ids 
 */
class Triangle
{
public:
  int a, b, c;

  inline Triangle(int v1, int v2, int v3) : a(v1), b(v2), c(v3) {};
  inline ~Triangle(){};

  inline int get(int i) const { int w[3] = {a,b,c}; return w[i]; };
  void boundary(const Mat &m, double& minX, double& minY, double& maxX, double& maxY) const;

  vector<Point2f> toFloatVector(const Mat& mat) const;
  vector<Point2d> toVector(const Mat& mat) const;
  void toIntArray(Point* p, const Mat& mat) const;
  Rect boundingRect(const Mat& m) const;

  // Triangle operator >>(const Point2d &displacement) const;
  // Triangle operator <<(const Point2d &displacement) const;
};


ostream &operator<<(ostream &os, Triangle const &m);

#endif
#ifndef AUX_UTIL
#define AUX_UTIL

#include "master.h"

namespace Aux
{
  Mat inline scaler(const Mat& m, double multiplier)
  {
    if (multiplier == 1.0) return m;
    else {
      int rows = (int)ceil(m.rows * multiplier);
      int cols = (int)ceil(m.cols * multiplier);
      Mat out  = Mat(rows, cols, m.type());
      resize(m, out, Size(rows, cols));
      return out;
    }
  }

  /**
   * Check whether the point [c]
   * is located inside the shape which has [vertices]
   */
  bool inline isInsideShape(const vector<Point2d>& vertices, Point2d c)
  {
    if (vertices.empty() || vertices.size()<=2)
      return false;
    auto v0 = vertices.front();
    int numIntersection = 0;
    for (auto v : vertices)
    {
      // Horizontal line test

      // Check if an edge intersects with the horizontal line starting from [c]
      if ((v0.x > c.x || v.x > c.x) && 
        (min(v0.y, v.y) <= c.y) &&
        (max(v0.y, v.y) >= c.y))
      {
        ++numIntersection;
      }
    }
    return numIntersection % 2 == 1;
  }
}

#endif
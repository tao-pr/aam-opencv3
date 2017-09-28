#ifndef AUX_UTIL
#define AUX_UTIL

#include "master.h" 
#include "math.h"

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

  Mat inline rotateByAngle(const Mat& m, double angle)
  {
    Mat R = (Mat_<float>(2,2) << cos(angle), -sin(angle), sin(angle), cos(angle));
    return R * m;
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
      v0 = v;
    }
    return numIntersection % 2 == 1;
  }

  inline double square(double n)
  {
    return n*n;
  }

  inline double _sqrDist(Point2d p1, Point2d p2)
  {
    return square(p1.x - p2.x) + square(p1.y - p2.y);
  };

  // Fast square root
  // [href] : http://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
  inline double sqrt(double n)
  {
    union
    {
      int i;
      double x;
    } u;
    u.x = n;
    u.i = (1<<29) + (u.i >> 1) - (1<<22); 

    // Two Babylonian Steps (simplified from:)
    // u.x = 0.5f * (u.x + n/u.x);
    // u.x = 0.5f * (u.x + n/u.x);
    u.x =       u.x + n/u.x;
    u.x = 0.25f*u.x + n/u.x;

    return u.x;
  }

  const Point2d inline centroid(const vector<Point2d>& vertices)
  {
    Point2d sum = Point2d(0,0);
    for (auto v : vertices)
    {
      sum += v;
    }
    sum.x /= (double)vertices.size();
    sum.y /= (double)vertices.size();
    return sum;
  }

  const double inline l2(const vector<Point2d>& vertices)
  {
    double sum = 0;
    for (auto v : vertices)
    {
      sum += v.x * v.x + v.y * v.y;
    }
    return Aux::sqrt(sum);
  }

  /**
   * Normalise a group of vertices,
   * so it results in a unit shape and translation removed.
   */
  const vector<Point2d> inline normalise(const vector<Point2d>& vertices)
  {
    Point2d mean;
    vector<Point2d> output;
    double norm = l2(vertices);
    Point2d v0  = vertices[0];

    for (auto v : vertices)
    {
      Point2d v_ = Point2d(
        (v.x - v0.x)/norm,
        (v.y - v0.y)/norm
      );
      output.push_back(v_);
    }
    return output;
  }

}

namespace Draw
{
  void drawTriangle(Mat canvas, Point2d a, Point2d b, Point2d c, Scalar color, int thickness=1, int mode=0)
  {
    line(canvas, a, b, color, thickness, mode);
    line(canvas, c, b, color, thickness, mode);
    line(canvas, a, c, color, thickness, mode);
  }
}

#endif
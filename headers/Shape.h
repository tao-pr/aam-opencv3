/**
 * Generic Shape Model
 */

#ifndef SHAPE_MODEL
#define SHAPE_MODEL

#include "master.h"
#include "IO.h"
#include "aux.h"

class Shape
{
private:
public:
  Mat mat;
  
  inline Shape(){};
  Shape(const vector<Point2d>& vs);
  Shape(const Mat &mat);
  inline Shape(const Shape& original){ original.mat.copyTo(mat); };
  inline virtual ~Shape(){};

  //----- General properties ------
  const Point2d& centroid() const;
  vector<Point2d> convexHull() const;
  const double sumSquareDistanceToPoint(const Point2d& p) const;

  //------ I/O ------
  virtual void save(const string path) const;
  virtual void load(const string path);
  vector<Point2d> toPoints() const;

  //------ Operators -------
  Shape operator-(const Shape& another) const; // Displacement between two shapes
  Shape operator+(const Shape& another) const; // Addition of two shapes
  Shape operator*(double scale) const; // Scaling
  Shape operator >>(Point2d shift) const;  // Translating
};

#endif
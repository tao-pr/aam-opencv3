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
protected:
  Mat mat;

public:
  Shape(){};
  Shape(vector<Point2d>& vs);
  Shape(const Mat &mat);
  inline Shape(const Shape& original){ this->mat.copyFrom(original.mat); };
  virtual ~Shape(){};

  //----- General properties ------
  size_t length() const { return this->vertices.size(); };
  const Point2d& centroid() const;
  vector<Point2d> convexHull() const;

  //------ I/O ------
  virtual void save(const string path) const;
  virtual void load(const string path);
  Mat toPoints() const;

  //------ Operators -------
  Shape operator-(const Shape& another) const; // Displacement between two shapes
  Shape operator+(const Shape& another) const; // Addition of two shapes
  Shape operator*(double scale) const; // Scaling
  Shape operator>>(Point2d shift) const;  // Translating
};



#endif
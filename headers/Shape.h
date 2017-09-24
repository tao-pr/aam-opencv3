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
  vector<Point2d> vertices; // Triangulated mesh vertices

public:
  Shape(){};
  Shape(vector<Point2d>& vs);
  Shape(const Mat &mat);
  inline Shape(const Shape& original) : vertices(original.vertices){};
  virtual ~Shape(){};

  //----- General properties ------
  size_t length() const { return this->vertices.size(); };
  const Point2d& centroid() const;
  vector<Point2d> convexHull() const;

  //------ I/O ------
  virtual void save(const string path) const;
  virtual void load(const string path);
  Mat toMat() const;

  //------ Operators -------
  Shape operator-(const Shape& another) const; // Shape square distance
  Shape operator+(const Shape& another) const; // Addition of two shapes
  Shape operator*(double scale) const; // Scaling
  Shape operator>>(Point2d shift) const;  // Translating
  virtual Shape normalise() const; // Remove scaling and translation from [Shape]
  double procrustesDistance(const Shape& another) const;
  
};



#endif
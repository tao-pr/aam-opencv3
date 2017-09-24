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
  Shape(const Shape& original);
  virtual ~Shape(){};

  //----- General properties ------
  size_t length() const { return this->vertices.size(); };
  const Point2d& centroid() const;
  const double centroidSize(const Point2d& precalculatedCentroid) const;
  vector<Point2d> convexHull() const;

  //------ I/O ------
  virtual void save(const string path) const;
  virtual void load(const string path);
  Mat toMat() const;

  //------ Operators -------
  Shape operator*(double scale) const; // Scaling
  Shape operator>>(Point2d shift) const;  // Translating
  virtual Shape normalise() const;
  virtual Shape alignTo(const Shape& s) const;
  double procrustesDistance(const Shape& another) const;
  
};



#endif
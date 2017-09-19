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
  Shape(const Shape& original);
  virtual ~Shape(){};

  //----- General properties ------
  size_t length() const { return this->vertices.size(); };
  const Point2d& meanXY() const;
  vector<Point2d> convexHull() const;

  //------ I/O ------
  virtual void save(const string path) const;
  virtual void load(const string path);

  //------ Operators -------
  Shape operator*(double scale) const; // Scaling
  Shape operator>>(Point2d shift) const;  // Translating
  Shape normalise() const;
  
};



#endif
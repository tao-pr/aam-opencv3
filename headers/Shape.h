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
  const int MAX_SHAPE_COMPONENTS = 16;

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
  void resize(const Size& newSize);
  Shape operator*(double scale) const;
  Shape operator>>(Point2d shift) const; 
  Shape normalise() const;
  
};



#endif
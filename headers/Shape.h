/**
 * Generic Shape Model
 */

#ifndef SHAPE_MODEL
#define SHAPE_MODEL

#include "master.h"
#include "Appearance.h"

class Shape
{
protected:
  vector<Appearance> a; // Base appearance
public:
  Shape(){};
  Shape(const vector<Appearance>& apps);
  virtual ~Shape(){};
  size_t size() const { return s.size() };

  Shape operator +(const Shape& that);
  Shape operator *(const double scale);

  static Shape createFromLinearCombination(Shape baseShape, vector<Shape> shapes, vector<double> params);
};



#endif
/**
 * Generic Shape Model
 */

#ifndef SHAPE_MODEL
#define SHAPE_MODEL

#include "master.h"
#include "aux.h"
#include "Appearance.h"

class Shape
{
protected:
  vector<Appearance> a; // Base appearance
public:
  Shape(){};
  Shape(const vector<Appearance>& apps);
  virtual ~Shape(){};
  size_t size() const { return a.size(); };

  Shape operator +(const Shape& that) const;
  Shape operator *(const double scale) const;

  void display(const Rect& bound, const string& wndName, double scaleFactor = 1.0) const;

  static Shape createFromLinearCombination(Shape baseShape, vector<Shape> shapes, vector<double> params);
};



#endif
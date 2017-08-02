/**
 * Generic Shape Model
 */

#ifndef SHAPE_MODEL
#define SHAPE_MODEL

#include "master.h"
#include "aux.h"

class Shape
{
protected:
  Rect constraint;
  vector<Point2d> vertices; // Triangulated mesh vertices
  Mat app; // Appearance image
  Subdiv2D subdiv;

public:
  Shape(){};
  Shape(const Rect& size);
  Shape(const Rect& size, const vector<Point2d>& vs);
  Shape(const Shape& original);
  virtual ~Shape(){};
  size_t length() const { return this->vertices.size(); };
  Rect size() const { return constraint; }

  // Summation of two [[Shape]] results in addition of appearances
  Shape operator +(const Shape& that) const;
  // Scaling of Appearance Model
  Shape operator *(const double scale) const;

  void setAppearance(const Mat& src);
  void resize(const Rect& newSize);
  void normalise();
  void applyParameters(const vector<double>& params);
  vector<Point> convexHull() const;

  void display(const string& wndName, double scaleFactor = 1.0) const;
};



#endif
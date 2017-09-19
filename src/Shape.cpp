#include "Shape.h"


Shape::Shape(const vector<Point2d>& vs)
{
  this->vertices = vs;
}

Shape::Shape(const Shape& original)
{
  this->vertices   = original.vertices;
  this->subdiv     = Subdiv2D(size);
  for (auto const v : this->vertices)
  {
    this->subdiv.insert(v);
  }
}

const Point2d& Shape::meanPos() const
{
  double x = 0;
  double y = 0;
  double n = (double)this->vertices.size();
  for (auto p : this->vertices)
  {
    x += p.x;
    y += p.y;
  }
  return Point2d(x/n, y/n);
}

vector<Point2d> Shape::convexHull() const
{
  vector<Point2d> hull;
  cv::convexHull(Mat(this->vertices), hull, false);
  return hull;
}

void Shape::resize(const Size& newSize)
{
  double wScale = (double)newSize.width / this->constraint.width;
  double hScale = (double)newSize.height / this->constraint.height;
  this->constraint = newSize;

  // Regenerate the subdivision
  Subdiv2D newsubdiv(newSize);
  for (auto v : this->vertices)
  {
    v.x *= wScale;
    v.y *= hScale;
    this->subdiv.insert(v);
  }
  this->subdiv = newsubdiv;

  // Scale the appearance image
  Mat newApp(newSize, this->app.type());
  resize(this->app, newApp, newSize);
  newApp.copyTo(this->app);
}

Shape operator*(double scale) const
{
  vector<Point2d> scaledVertices;
  for (auto v : this->vertices)
  {
    scaledVertices.push_back(Point2d(v.x*scale, v.y*scale));
  }
  return new Shape(scaledVertices);
}

Shape operator>>(Point2d shift) const
{
  vector<Point2d> shiftedVertices;
  for (auto v : this->vertices)
  {
    shiftedVertices.push_back(Point2d(v.x+shift.x, v.y+shift.y));
  }
  return new Shape(shiftedVertices);
}

Shape Shape::normalise() const
{
  auto normalisedVertices = Aux::normalise(this->vertices);
  return new Shape(normalisedVertices);
}




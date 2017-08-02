#include "Shape.h"

Shape::Shape(const Rect& size)
{
  this->constraint = size;
  this->subdiv     = Subdiv2D(size);
}

Shape::Shape(const Rect& size, const vector<Point2d>& vs)
{
  this->constraint = size;
  this->vertices   = vs;
  this->subdiv     = Subdiv2D(size);
  for (auto const v : this->vertices)
  {
    this->subdiv.insert(v);
  }
}

Shape::Shape(const Shape& original)
{
  this->constraint = original.constraint;
  this->vertices   = original.vertices;
  this->subdiv     = Subdiv2D(size);
  for (auto const v : this->vertices)
  {
    this->subdiv.insert(v);
  }
}

Shape Shape::operator +(const Shape& that) const
{
  vector<Appearance> aOut;
  auto bi = that.a.begin();
  for (auto ai : this->a)
  {
    aOut.push_back(ai + *bi);
    ++bi;
  }
  return Shape(aOut);
}

Shape Shape::operator *(const double scale) const
{
  vector<Appearance> aOut;
  for (auto ai : this->a)
  {
    aOut.push_back(ai * scale);
  }
  return Shape(aOut);
}

void Shape::display(const string& wndName, double scaleFactor) const
{
  Size scaledBound = Size(
    (int)ceil(scaleFactor * this->constraint.width),
    (int)ceil(scaleFactor * this->constraint.height)
  );
  Mat canvas = Mat::zeros(scaledBound.height, scaledBound.width);
  for (auto app : this->a)
  {
    // TAOTODO: 
  }

  imshow(canvas, wndName);
}

void Shape::resize(const Rect& newSize)
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

  // TAOTODO: Scale the appearance image here

}

void Shape::normalise()
{
  this->resize(Rect(1.0, 1.0));
}
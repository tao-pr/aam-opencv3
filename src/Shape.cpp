#include "Shape.h"

Shape::Shape(const vector<Appearance>& apps)
{
  this->a = apps;
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

Shape Shape::createFromLinearCombination(Shape baseShape, vector<Shape> shapes, vector<double> params)
{
  int i = 0;
  auto product = baseShape;
  for (auto s : shapes) 
  {
    product = product + s * params[i];
    ++i;
  }
  return product;
}
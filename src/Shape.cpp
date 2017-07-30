#include "Shape.h"

Shape::Shape(const vector<Appearance>& apps)
{
  swap(this->a, apps);
}

Shape Shape::operator +(const Shape& that)
{
  vector<Appearance> aOut;
  auto bi = that.a->begin();
  for (auto ai : this->a)
  {
    aOut.push_back(ai + *bi);
    ++bi;
  }
  return Shape(aOut);
}

Shape Shape::operator *(const double scale)
{

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
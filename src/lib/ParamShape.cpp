#include "ParamShape.h"

Shape ShapeEncoder::decode(const ParameterisedShape &s) const
{
  return Shape(this->mean + this->eigen * s.getParams());
}

Mat ShapeEncoder::encode(const Shape& s) const
{
  return this->eigen_1 * (s.mat - this->mean);
}

ParameterisedShape::ParameterisedShape(const Shape& shape, const ShapeEncoder& enc)
{
  this->params = enc.encode(shape);
}

Shape ParameterisedShape::toShape(const ShapeEncoder& enc) const
{
  return enc.decode(*this);
}
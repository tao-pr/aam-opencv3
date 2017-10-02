#include "ParamShape.h"

/**
 * Convert a parameter set to a shape
 */
Shape ShapeEncoder::decode(const ParameterisedShape &s) const
{
  return Shape(this->mean + (this->eigen * s.getParams()));
}

/**
 * Convert a shape to a parameter set
 */
Mat ShapeEncoder::encode(const Shape& s) const
{
  // (Eigen^-1)•(shape - mean)
  return this->eigen_1 * (s.toColVector() - this->mean);
}

ParameterisedShape::ParameterisedShape(const Shape& shape, const ShapeEncoder& enc)
{
  this->params = enc.encode(shape);
}

Shape ParameterisedShape::toShape(const ShapeEncoder& enc) const
{
  return enc.decode(*this);
}
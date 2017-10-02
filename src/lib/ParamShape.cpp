#include "ParamShape.h"

Shape ShapeEncoder::decode(const ParameterisedShape &s) const
{
  return Shape(this->mean + this->eigen * s.getParams());
}

Mat ShapeEncoder::encode(const Shape& s) const
{
  int N = s.mat.rows;
  // (Eigen^-1)•(shape - mean)
  Mat residue = (s.toRowVector() - this->mean);
  Mat shapeMat = this->eigen_1 * residue.t();
  return shapeMat.reshape(1, N);
}

ParameterisedShape::ParameterisedShape(const Shape& shape, const ShapeEncoder& enc)
{
  this->params = enc.encode(shape);
}

Shape ParameterisedShape::toShape(const ShapeEncoder& enc) const
{
  return enc.decode(*this);
}
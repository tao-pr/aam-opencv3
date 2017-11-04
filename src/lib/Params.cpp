#include "Params.h"

/**
 * Convert a parameter set to a shape
 */
Shape ModelEncoder::toShape(const ModelParameters &s) const
{
  // shape = mean + (Eigen•params)
  return Shape(this->mean + (this->eigen * s.getParams()));
}

/**
 * Convert a shape to a parameter set
 */
Mat ModelEncoder::encode(const Shape& s) const
{
  // params = (Eigen^-1)•(shape - mean)
  return this->eigen_1 * (s.toColVector() - this->mean);
}

ModelParameters::ModelParameters(const Shape& shape, const ModelEncoder& enc)
{
  this->params = enc.encode(shape);
}

ModelParameters::ModelParameters(const Appearance& app, const ModelEncoder& enc)
{
  this->params = enc.encode(app);
}

Shape ModelParameters::toShape(const ModelEncoder& enc) const
{
  return enc.toShape(*this);
}

Appearance ModelParameters::toAppearance(const ModelEncoder& enc, const Shape& shape, const Mat* img) const
{
  return enc.toAppearance(*this, shape, img);
}
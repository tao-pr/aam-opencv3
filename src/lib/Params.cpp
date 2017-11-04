#include "Params.h"

/**
 * Convert a parameter set to a shape
 */
Shape ModelEncoder::toShape(const ModelParameters &s) const
{
  // shape = mean + (Eigen•params)
  return Shape(this->mean + (this->eigen * s.getParams()));
}

tuple<Appearance,Mat*> ModelEncoder::toAppearance(const ModelParameters &s, const MeshShape& meanShape) const
{
  // appearance = mean + (Eigen•params)

  // Convert column vector of appearance to a spatial mat
  // Also round the floating points of pixel values to 8-bit ints
  auto bound  = meanShape.getBound();
  Mat* spatial = new Mat(bound.size(), CV_8UC3, Scalar(0,0,0));

  const int xA = bound.x;
  const int xB = bound.x + bound.width;
  const int yA = bound.y;
  const int yB = bound.y + bound.height;

  for (int j=yA; j<yB; j++)
    for (int i=xA; i<xB; i++)
    {
      // Draw if the pixel lies within the convex hull of the shape
      // TAOTODO:
    }

  auto app    = Appearance(meanShape, spatial);
  return make_tuple(app, spatial);
}

Mat ModelEncoder::encode(const GenericModel& m) const
{
  // params = (Eigen^-1)•(shape - mean)
  return this->eigen_1 * (m.toColVector() - this->mean);
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

tuple<Appearance,Mat*> ModelParameters::toAppearance(const ModelEncoder& enc, const MeshShape& meanShape) const
{
  return enc.toAppearance(*this, meanShape);
}
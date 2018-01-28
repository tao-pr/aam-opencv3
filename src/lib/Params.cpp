#include "Params.h"

Mat ModelEncoder::safeInverse(const Mat& m)
{
  if (m.cols == m.rows) return m.inv();
  else 
  {
    int n = min(m.cols, m.rows);
    Mat m_ = m(Rect(0,0,n,n));
    #ifdef DEBUG
    cout << "safeInverse : instead of " << m.size() << ", this computes " << m_.size() << endl;
    #endif
    return m_.inv();
  }
}

/**
 * Convert a parameter set to a shape
 */
Shape ModelEncoder::toShape(const ModelParameters &s) const
{
  // shape = mean + (Eigen•params)
  return Shape(this->mean + (this->eigen * s.getParams()));
}

Appearance ModelEncoder::toAppearance(const ModelParameters &s, const MeshShape* meanShape) const
{
  // appearance = mean + (Eigen•params)

  // Convert column vector of appearance to a spatial mat
  // Also round the floating points of pixel values to 8-bit ints
  auto bound  = meanShape->getBound();
  auto mtype  = this->mean.channels() == 3 ? CV_8UC3 : CV_8UC1;
  Mat spatial = Mat(bound.size(), CV_8UC3, Scalar(0,0,0));

  this->mean.reshape(this->mean.channels(), bound.height).copyTo(spatial);
  auto app    = Appearance(*meanShape, spatial);
  return app;
}

Mat ModelEncoder::encode(const BaseModel* m) const
{
  // params = (Eigen^-1)•(shape - mean)
  return this->eigen_1 * (m->toColVector() - this->mean);
}

ModelParameters::ModelParameters(const Shape& shape, const ModelEncoder& enc)
{
  this->params = enc.encode(&shape);
}

ModelParameters::ModelParameters(const Appearance& app, const ModelEncoder& enc)
{
  this->params = enc.encode(&app);
}

Shape ModelParameters::toShape(const ModelEncoder& enc) const
{
  return enc.toShape(*this);
}

tuple<Appearance,Mat*> ModelParameters::toAppearance(const ModelEncoder& enc, const MeshShape* meanShape) const
{
  return enc.toAppearance(*this, meanShape);
}
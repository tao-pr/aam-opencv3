#include "Params.h"

// Mat ModelEncoder::safeInverse(const Mat& m)
// {
//   if (m.cols == m.rows) return m.inv();
//   else 
//   {
//     int n = min(m.cols, m.rows);
//     Mat m_ = m(Rect(0,0,n,n));
//     #ifdef DEBUG
//     cout << "safeInverse : instead of " << m.size() << ", this computes " << m_.size() << endl;
//     #endif
//     return m_.inv();
//   }
// }

/**
 * Convert a parameter set to a shape
 */
Shape ModelEncoder::toShape(const ModelParameters &s) const
{
  // shape = mean + (Eigen•params)
  // return Shape(this->mean + (this->eigen * s.getParams()));

  Mat inputParams = s.getParams();
  cout << "input params : " << inputParams.size() << endl; // TAODEBUG:
  Mat coords = this->pca.backProject(inputParams);
  cout << "back projected : " << coords.size() << endl; // TAODEBUG:
  return Shape(coords);
}

Appearance ModelEncoder::toAppearance(const ModelParameters &s, const MeshShape* meanShape) const
{
  // appearance = mean + (Eigen•params)

  // Convert column vector of appearance to a spatial mat
  // Also round the floating points of pixel values to 8-bit ints
  
  // auto bound  = meanShape->getBound();
  // auto mtype  = this->mean.channels() == 3 ? CV_8UC3 : CV_8UC1;
  // Mat spatial = Mat(bound.size(), CV_8UC3, Scalar(0,0,0));

  // this->mean.reshape(this->mean.channels(), bound.height).copyTo(spatial);
  // auto app    = Appearance(*meanShape, spatial);
  // return app;

  auto bound = meanShape->getBound();
  auto mtype  = this->mean.channels() == 3 ? CV_8UC3 : CV_8UC1;
  Mat spatial = Mat(bound.size(), CV_8UC3, Scalar(0,0,0));
}

Mat ModelEncoder::encode(const BaseModel* m) const
{
  // If the eigen vector is not square
  // reduce the dimension of column vector of the model
  // NOTE: only supports [[Appearance]]
  if (this->eigen.cols != this->eigen.rows)
  {
    const Appearance* app = dynamic_cast<const Appearance*>(m);
    Mat colVec = app->toRowVectorReduced(this->eigen_1.rows).t();

    // TAODEBUG:
    cout << "encoding appearance" << endl;
    cout << "inverse eigen : " << this->eigen_1.size() << endl;
    cout << "col vec       : " << colVec.size() << endl;
    cout << "mean          : " << this->mean.size() << endl;
    
    // params = (Eigen^-1)•(shape - mean)
    return this->eigen_1 * (colVec - this->mean);
  }
  else
  {
    // Conventional approach
    // Stored eigen vectors are simply square matrix
    // params = (Eigen^-1)•(shape - mean)
    return this->eigen_1 * (m->toColVector() - this->mean);
  }
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
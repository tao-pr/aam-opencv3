#ifndef PARAM_SHAPE
#define PARAM_SHAPE

#include "master.h"
#include "Shape.h"

// TAOTODO: Make following more generic, so they support both Shape & Texture
class ModelParameters;

class ModelEncoder
{
protected:
  Mat mean; // Mean column vector
  Mat eigen;
  Mat eigen_1; // Invert of eigen matrix (NxN)
public:
  ModelEncoder(Mat mean, Mat eigen) : mean(mean), eigen(eigen), eigen_1(eigen.inv()) {};
  virtual ~ModelEncoder(){};
  Shape decode(const ModelParameters &s) const;
  Mat encode(const Shape& s) const;
};

/**
 * PCA parameters of a model
 */
class ModelParameters
{
private:
protected:
  Mat params; // Column vector (Nx1)
public:
  ModelParameters(const Shape& shape, const ModelEncoder& enc);
  ModelParameters(const Mat& params) : params(params) {};
  inline virtual ~ModelParameters(){};

  Mat getParams() const { return this->params; };

  // ---------- Conversion -----------
  Shape toShape(const ModelEncoder& enc) const;
};

#endif
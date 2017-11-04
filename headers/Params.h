#ifndef PARAM_SHAPE
#define PARAM_SHAPE

#include "master.h"
#include "GenericModel.h"
#include "Shape.h"
#include "AppearanceModel.h"

class ModelParameters;

/**
 * Common encoder for the model.
 * Multiple models may share the same [[ModelEncoder]].
 */
class ModelEncoder
{
protected:
  Mat mean; // Mean column vector
  Mat eigen;
  Mat eigen_1; // Invert of eigen matrix (NxN)
public:
  ModelEncoder(Mat mean, Mat eigen) : mean(mean), eigen(eigen), eigen_1(eigen.inv()) {};
  virtual ~ModelEncoder(){};
  Shape toShape(const ModelParameters &s) const;
  Appearance toAppearance(const ModelParameters &s) const;
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
  ModelParameters(const Appearance& app, const ModelEncoder& enc);
  ModelParameters(const Mat& params) : params(params) {};
  inline virtual ~ModelParameters(){};

  Mat getParams() const { return this->params; };

  // ---------- Conversion -----------
  Shape toShape(const ModelEncoder& enc) const;
  Appearance toAppearance(const ModelEncoder& enc) const;
};

#endif
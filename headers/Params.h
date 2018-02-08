#ifndef PARAM_SHAPE
#define PARAM_SHAPE

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "Appearance.h"

// TAOTODO: Deprecate this module
class ModelParameters;

/**
 * Common encoder for the model.
 * Multiple models may share the same [[ModelEncoder]].
 */
class ModelEncoder // TAOTODO: Store [[PCA]] object here instead
{
protected:
  Mat mean; // Mean column vector
  Mat eigen;
  Mat eigen_1; // Invert of eigen matrix (NxN)
  PCA pca;
public:
  ModelEncoder(PCA& p) : pca(p) {};
  //ModelEncoder(Mat mean, Mat eigen) : mean(mean), eigen(eigen), eigen_1(ModelEncoder::safeInverse(eigen)) {};
  virtual ~ModelEncoder(){};
  
  Shape toShape(const ModelParameters &s) const;
  Appearance toAppearance(const ModelParameters &s, const MeshShape* meanShape) const;
  Mat encode(const BaseModel* m) const;

  // static Mat safeInverse(const Mat& m);
};

/**
 * PCA parameters of a model instance
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
  tuple<Appearance,Mat*> toAppearance(const ModelEncoder& enc, const MeshShape* meanShape) const;
};

#endif
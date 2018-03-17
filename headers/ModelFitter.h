#ifndef MODEL_FITTER
#define MODEL_FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"
#include "BaseFittedModel.h"

class FittedAAM;

struct FittingCriteria
{
  int numMaxIter;
  double eps;
  double initScale;
  Point2d initPos;

  static FittingCriteria getDefault()
  {
    return FittingCriteria{ 10, 1e-3, 1, Point2d(0,0) };
  };
};

class ModelFitter
{
private:
protected:
  // Static PCA of Shape and Appearance components
  ShapeModelPCA pcaShape;
  AppearanceModelPCA pcaAppearance;

  virtual BaseFittedModel& initModel(const FittingCriteria& crit) const;

public:
  ModelFitter(const ShapeModelPCA& shapePCA, const AppearanceModelPCA& appearancePCA)
    : pcaShape(shapePCA), pcaAppearance(appearancePCA) {};
  virtual ~ModelFitter(){};

  const ShapeModelPCA getShapePCA() { return this->pcaShape; } const;
  const AppearanceModelPCA getAppearancePCA() { return this->pcaAppearance; } const;

  virtual BaseFittedModel& fit(const Mat& sample, const FittingCriteria& crit = FittingCriteria::getDefault()) const;
};

#endif
#ifndef MODEL_FITTER
#define MODEL_FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"
#include "FittedAAM.h"

class ModelFitter
{
private:
protected:
  // Static PCA of Shape and Appearance components
  ShapeModelPCA pcaShape;
  AppearanceModelPCA pcaAppearance;

public:
  ModelFitter(const ShapeModelPCA& shapePCA, const AppearanceModelPCA& appearancePCA)
    : pcaShape(shapePCA), pcaAppearance(appearancePCA) {};
  virtual ~ModelFitter(){};

  const ShapeModelPCA getShapePCA() { return this->pcaShape; } const;
  const AppearanceModelPCA getAppearancePCA() { return this->pcaAppearance; } const;
};

#endif
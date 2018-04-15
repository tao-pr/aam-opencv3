#ifndef MODEL_FITTER
#define MODEL_FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"
#include "BaseFittedModel.h"

enum SearchWith
{
  SCALING = 0,
  TRANSLATION,
  RESHAPING,
  REAPPEARANCING
  // TAOTOREVIEW: Rotation?
};

struct FittingCriteria
{
  int numMaxIter;
  double eps;
  double initScale;
  Point2d initPos; // Coordinate of the upper-left origin

  static FittingCriteria getDefault()
  {
    return FittingCriteria{ 10, 1e-3, 100, Point2d(0,0) };
  };
};

class ModelFitter
{
private:
  ModelFitter(const ModelFitter& another);

protected:
  // Static PCA of Shape and Appearance components
  unique_ptr<AAMPCA> aamPCA;
  unique_ptr<BaseFittedModel> generateNextBestModel(unique_ptr<BaseFittedModel> const& model, const Mat& sample, double* bestError) const;

public:
  inline ModelFitter(unique_ptr<AAMPCA> const & aamPCA)
  {
    this->aamPCA = aamPCA->clone();
  };
  
  virtual inline ~ModelFitter()
  {
    this->aamPCA.reset();
  };

  const ShapeModelPCA getShapePCA() { return aamPCA->getShapePCA(); } const;
  const AppearanceModelPCA getAppearancePCA() { return aamPCA->getAppearancePCA(); } const;

  virtual unique_ptr<BaseFittedModel> fit(unique_ptr<BaseFittedModel>& initModel, const Mat& sample, const FittingCriteria& crit = FittingCriteria::getDefault()) const;
};

#endif
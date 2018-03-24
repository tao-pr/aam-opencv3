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
protected:
  // Static PCA of Shape and Appearance components
  shared_ptr<AAMPCA> aamPCA;
  tuple<BaseFittedModel*, double> generateNextBestModel(BaseFittedModel* model, const Mat& sample) const;

public:
  inline ModelFitter(shared_ptr<AAMPCA> const & aamPCA) : aamPCA(aamPCA){};
  virtual ~ModelFitter(){};

  const ShapeModelPCA getShapePCA() { return aamPCA->getShapePCA(); } const;
  const AppearanceModelPCA getAppearancePCA() { return aamPCA->getAppearancePCA(); } const;

  virtual BaseFittedModel* fit(const BaseFittedModel* initModel, const Mat& sample, const FittingCriteria& crit = FittingCriteria::getDefault()) const;
};

#endif
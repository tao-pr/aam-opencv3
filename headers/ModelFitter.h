#ifndef MODEL_FITTER
#define MODEL_FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"
#include "PriorityLinkedList.h"

typedef PriorityLinkedList<BaseFittedModel> ModelList;

struct GenerateModels;

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
  int maxTreeSize;
  int numModelsToGeneratePerIter;
  double minError;
  double initScale;
  Point2d initPos; // Coordinate of the upper-left origin

  static FittingCriteria getDefault()
  {
    return FittingCriteria{ 10, 16, 8, 1e-4, 100, Point2d(0,0) };
  };
};

class ModelFitter
{
private:
  ModelFitter(const ModelFitter& another);

protected:
  // Static PCA of Shape and Appearance components
  unique_ptr<AAMPCA> aamPCA;

public:
  inline ModelFitter(unique_ptr<AAMPCA> const & aamPCA)
  {
    this->aamPCA = aamPCA->clone();
  };
  
  virtual inline ~ModelFitter()
  {
    this->aamPCA.reset();
  };

  void generateNextBestModels(
    unique_ptr<ModelList>& container,
    double prevError, 
    BaseFittedModel const* model, 
    const Mat& sample, 
    int numModels = 1) const;

  const ShapeModelPCA& getShapePCA() const { return aamPCA->getShapePCA(); };
  const AppearanceModelPCA& getAppearancePCA() const { return aamPCA->getAppearancePCA(); };

  virtual unique_ptr<BaseFittedModel> fit(unique_ptr<BaseFittedModel>& initModel, const Mat& sample, const FittingCriteria& crit = FittingCriteria::getDefault()) const;
};

struct GenerateModels
{
  void operator()(
    unique_ptr<ModelList>& mlist, 
    ModelFitter* fitter, 
    Mat& sample,
    int numModelsToGen,
    BaseFittedModel* m, 
    double v) const
  {
    fitter->generateNextBestModels(
      mlist,
      v,
      m,
      sample,
      numModelsToGen
    );
  };
};

#endif
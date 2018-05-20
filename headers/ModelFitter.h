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
  FittingCriteria crit;
  unique_ptr<AAMPCA> aamPCA;
  ModelList models;
  ModelList buffer;
  Mat sample;

  void iterateModelExpansion(ModelList* const modelPtr);

public:
  inline ModelFitter(
    unique_ptr<AAMPCA> const & aamPCA,
    FittingCriteria const& crit,
    Mat& sample) 
    : crit(crit)
    {
      this->aamPCA = aamPCA->clone();
      sample.copyTo(this->sample);
    };
  
  virtual inline ~ModelFitter()
  {
    this->aamPCA.reset();
  };

  void setSample(Mat& sample)
  {
    sample.copyTo(this->sample);
  };

  void setCriteria(FittingCriteria& crit)
  {
    this->crit = crit;
  };

  const ShapeModelPCA& getShapePCA() const { return aamPCA->getShapePCA(); };
  const AppearanceModelPCA& getAppearancePCA() const { return aamPCA->getAppearancePCA(); };

  virtual unique_ptr<BaseFittedModel> fit(unique_ptr<BaseFittedModel>& initModel);
};


#endif
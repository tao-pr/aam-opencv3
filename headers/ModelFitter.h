#ifndef MODEL_FITTER
#define MODEL_FITTER

#include "master.h"
#include "Try.h"
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

const double SCALING_MIN = 0.677;
const double SCALING_MAX = 3;
const double TRANSLATION_MIN = -250;
const double TRANSLATION_MAX = 250;
const double RESHAPING_MIN = -50;
const double RESHAPING_MAX = 50;
const double REAPPEARANCING_MIN = -50;
const double REAPPEARANCING_MAX = 50;

struct FittingCriteria
{
  int numMaxIter;
  int maxTreeSize;
  int numModelsToGeneratePerIter;
  double minErrorImprovement;
  double initScale;
  Point2d initPos; // Coordinate of the upper-left origin
  double minScale;
  double maxScale;

  static FittingCriteria getDefault()
  {
    return FittingCriteria{ 10, 16, 8, 1e-4, 100, Point2d(0,0), 0.33, 3 };
  };
};

class ModelFitter
{
private:
  ModelFitter(const ModelFitter& another);

  Mat* smat;
  Mat* amat;
  int smatSize;
  int amatSize;
  ShapeModelPCA pcaShape;
  AppearanceModelPCA pcaAppearance;
  static const double scales[5];
  static const Point2d trans[9];

protected:
  // Static PCA of Shape and Appearance components
  FittingCriteria crit;
  unique_ptr<AAMPCA> aamPCA;
  ModelList models;
  ModelList buffer;
  Mat sample;
  Mat zero;

  void iterateModelExpansion(
    ModelList* const modelPtr,
    SearchWith action = TRANSLATION,
    double scale = 1.0);
  
  void transferFromBuffer(int nLeft);
  void buildCache();

public:
  inline ModelFitter(
    unique_ptr<AAMPCA> const & aamPCA,
    FittingCriteria const& crit,
    Mat& sample) 
    : crit(crit)
    {
      this->aamPCA = aamPCA->clone();
      sample.copyTo(this->sample);
      zero = Mat::zeros(sample.size(), CV_8UC3);
      buildCache();
    };
  
  virtual inline ~ModelFitter()
  {
    this->aamPCA.reset();
    delete [] this->amat;
    delete [] this->smat;
  };

  void setSample(Mat& sample)
  {
    sample.copyTo(this->sample);
    zero = Mat::zeros(sample.size(), CV_8UC3);
  };

  void setCriteria(FittingCriteria& crit)
  {
    this->crit = crit;
  };

  const ShapeModelPCA& getShapePCA() const { return aamPCA->getShapePCA(); };
  const AppearanceModelPCA& getAppearancePCA() const { return aamPCA->getAppearancePCA(); };

  virtual unique_ptr<BaseFittedModel> fit(unique_ptr<BaseFittedModel>& initModel, int skipPixels);
};


#endif
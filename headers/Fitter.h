#ifndef FITTER
#define FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"

/**
 * Model parameters and the fitting statuses, e.g iteration number, errors etc
 */
struct FittedState 
{
  // Current states
  int iters;
  double error;
  
  // Fixed PCA characteristics
  ShapeModelPCA pcaShape;
  AppearanceModelPCA pcaAppearance;

  // Current states
  Mat shapeParam;
  Mat appParam;

  double eps(double oldError)
  {
    if (oldError == error)
      return 0;
    else if (oldError == 0)
      return numeric_limits<double>::max();
    else
      return abs(error - oldError)/oldError;
  }

  double measureError(const Mat& sample)
  {
    int M = pcaAppearance.dimension();

    // Generate row vector of the fitted appearance on the real sample
    auto meanShape = pcaShape.toShape(shapeParam);
    auto sampleApp = Appearance(*meanShape, sample);
    Mat sampleVec  = sampleApp.toRowVectorReduced(M);

    // Generate row vector of the candidate model
    Mat candVec = pcaAppearance.toAppearance(appParam)->toRowVectorReduced(M);

    // Compute (pseudo) MSE between two vectors
    double e = 0;
    for (int i=0; i<M; i++)
    {
      double x  = sampleVec.at<double>(0,i);
      double x0 = candVec.at<double>(0,i);
      e += abs(x - x0);
    }
    return e;
  }

  static FittedState create(const ShapeModelPCA& pcaShape, const AppearanceModelPCA& pcaAppearance)
  {
    double maxError = numeric_limits<double>::max();
    Mat shapePar = Mat::zeros(1, pcaShape.dimension(), CV_64FC1);
    Mat appPar   = Mat::zeros(1, pcaAppearance.dimension(), CV_64FC1);
    return FittedState{ 0, maxError, pcaShape, pcaAppearance, shapePar, appPar };
  };
};

/**
 * Basic model parameter optimiser
 */
class AAMFitter 
{
private:
protected:
  int maxIter;
  double eps;

  virtual FittedState fitIterNext(const Mat& sample, FittedState& fitState);
  virtual FittedState findBestTranslation(const Mat& sample, FittedState& fitState, double stepSize = 10);
  virtual FittedState findBestScaling(const Mat& sample, FittedState& fitState, double stepSize = 1);
  virtual FittedState findBestShapeMove(const Mat& sample, FittedState& fitState, double stepSize = 1e-3) const;
  virtual FittedState findBestAppearanceMove(const Mat& sample, FittedState& fitState, double stepSize = 1e-3) const;

public:
  inline AAMFitter(int maxIter, double eps = 1e-3) : maxIter(maxIter), eps(eps) {};
  inline virtual ~AAMFitter() {};

  virtual FittedState fit(Mat sample, const ShapeModelPCA& pcaShape, const AppearanceModelPCA& pcaAppearance);
};

#endif
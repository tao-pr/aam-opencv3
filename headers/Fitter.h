#ifndef FITTER
#define FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "Appearance.h"
#include "ModelPCA.h"

/**
 * Model parameters and the fitting statuses, e.g iteration number, errors etc
 */
struct FittedState 
{
  int iters;
  double error;
  ShapeModelPCA stateShape;
  AppearanceModelPCA stateAppearance;

  double eps(double oldError)
  {
    if (oldError == error)
      return 0;
    else if (oldError == 0 || error == 0)
      return numeric_limits<double>::max();
    else
      return abs(error - oldError)/oldError;
  }

  static FittedState create(const ShapeModelPCA& pcaShape, const AppearanceModelPCA& pcaAppearance)
  {
    double maxError = numeric_limits<double>::max();
    return FittedState{ 0, maxError, pcaShape, pcaAppearance };
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

  virtual FittedState fitIterNext(Mat sample, FittedState& fitState) = 0;
public:
  inline AAMFitter(int maxIter, double eps = 1e-3) : maxIter(maxIter), eps(eps) {};
  inline virtual ~AAMFitter() {};

  virtual FittedState fit(Mat sample, const ShapeModelPCA& pcaShape, const AppearanceModelPCA& pcaAppearance);
};

#endif
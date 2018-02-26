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
  ModelPCA *state;

  double eps(double newError)
  {
    if (newError == error)
      return 0;
    else if (newError == 0 || error == 0)
      return numeric_limits<double>::max();
    else
      return abs(error - newError)/error;
  }

  bool isValid() const { return state != nullptr; };
};

/**
 * Model parameter optimiser
 */
class ModelFitter 
{
private:
protected:
  int maxIter;
  double eps;

  virtual FittedState fitIterNext(Mat sample, FittedState& fitState) = 0;
public:
  inline ModelFitter(int maxIter, double eps = 1e-3) : maxIter(maxIter), eps(eps) {};
  inline virtual ~ModelFitter() {};

  virtual FittedState fit(Mat sample);
};


#endif
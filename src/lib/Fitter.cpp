#include "Fitter.h"

FittedState AAMFitter::fit(Mat sample, const ShapeModelPCA& pcaShape, const AppearanceModelPCA& pcaAppearance)
{
  // Repeatedly fit the model until it reaches the stopping criterion
  bool stopping = false;
  FittedState state = FittedState::create(pcaShape, pcaAppearance);
  while (!stopping)
  {
    double prevError = state.error;
    #ifdef DEBUG
    cout << CYAN << "[Fitting model] " << RESET << "iter #" << state.iters << ", error = " << state.error << endl;
    #endif
    
    state = fitIterNext(sample, state);

    stopping = state.iters >= this->maxIter || state.eps(prevError) <= eps;
  }

  #ifdef DEBUG
  cout << RED << "[Fitting model] ends with iter#" << state.iters << ", error = " << state.error << RESET << endl;
  #endif

  return state;
}

FittedState AAMFitter::fitIterNext(Mat sample, FittedState& fitState)
{
  // TAOTODO: Implement line search
  // auto meanShape = stateShape.toShape(Mat::zeros(1, stateShape.dimension(), CV_64FC1));


  fitState.iters ++;
  return fitState;
}
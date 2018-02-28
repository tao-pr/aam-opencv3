#include "Fitter.h"

FittedState ModelFitter::fit(Mat sample)
{
  // Repeatedly fit the model until it reaches the stopping criterion
  bool stopping = false;
  FittedState state = { 0, numeric_limits<double>::max(), nullptr };
  while (!stopping)
  {
    #ifdef DEBUG
    cout << CYAN << "[Fitting model] " << RESET << "iter #" << state.iters << ", error = " << state.error << endl;
    #endif
    
    state = fitIterNext(sample, state);
  }

  #ifdef DEBUG
  cout << RED << "[Fitting model] ends with iter#" << state.iters << ", error = " << state.error << RESET << endl;
  #endif

  return state;
}

FittedState AppearanceFitter::fitIterNext(Mat sample, FittedState& fitState)
{
  // TAOTODO:
}
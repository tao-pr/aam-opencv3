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

FittedState AAMFitter::findBestShapeMove(const Mat& sample, FittedState& fitState, double stepSize) const 
{
  // TAOTODO: Skip the prev step, otherwise it would bounce
  int M = fitState.stateShape.dimension();

  Mat bestParamDiff;
  double bestError = numeric_limits<double>::max();

  // Exhaustive step search
  double scalers[] = {-1, 1};
  for (int m=0; m<M; m++)
  {
    for (double scale : scalers)
    {
      Mat g = Mat::zeros(1, M, CV_64FC1);
      g.at<double>(0,m) = scale*stepSize;

      FittedState newState = fitState;
      newState.shapeParam += g;
      
      double e = newState.measureError(sample);
      if (e < bestError)
      {
        #ifdef DEBUG
        cout << "... [found new best shape move] : #" << m << " : " << scale
          << ", E = " << e << endl;
        #endif

        bestError = e;
        bestParamDiff = g;
      }
    }
  }

  FittedState newState = fitState;
  newState.shapeParam += bestParamDiff;
  newState.error = bestError;
  return newState;
}

FittedState AAMFitter::findBestAppearanceMove(const Mat& sample, FittedState& fitState, double stepSize) const
{
  int M = fitState.stateAppearance.dimension();

  Mat bestParamDiff;
  double bestError = numeric_limits<double>::max();

  // Exhaustive step search
  double scalers[] = {-1, 1};
  for (int m=0; m<M; m++)
  {
    for (double scale : scalers)
    {
      Mat g = Mat::zeros(1, M, CV_64FC1);
      g.at<double>(0,m) = scale*stepSize;

      FittedState newState(fitState);
      newState.appParam += g;
      
      double e = newState.measureError(sample);
      if (e < bestError)
      {
        #ifdef DEBUG
        cout << "... [found new best app move] : #" << m << " : " << scale
          << ", E = " << e << endl;
        #endif

        bestError = e;
        bestParamDiff = g;
      }
    }
  }

  FittedState newState(fitState);
  newState.appParam += bestParamDiff;
  newState.error = bestError;
  return newState;
}

FittedState AAMFitter::fitIterNext(const Mat& sample, FittedState& fitState)
{
  // TAOTODO: Implement line search
  
  // Find the best scaling and translation


  // Find the best shape parameters


  // Find the best appearance parameters


  fitState.iters ++;
  return fitState;
}
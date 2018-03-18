#include "ModelFitter.h"

tuple<BaseFittedModel*, double> ModelFitter::generateNextBestModel(BaseFittedModel* model, const Mat& sample) const
{
  // TAOTODO:
}

BaseFittedModel* ModelFitter::fit(const BaseFittedModel* initModel, const Mat& sample, const FittingCriteria& crit) const 
{
  double errorDiff = numeric_limits<double>::max();
  int iter = 0;
  
  #ifdef DEBUG
  cout << GREEN << "[Model fitting started]" << RESET << endl;
  #endif

  // Start with the given initial model
  auto prevModel = initModel->clone();

  // Adjust model parameters until converges
  while (iter < crit.numMaxIter && errorDiff > crit.eps)
  {
    #ifdef DEBUG
    cout << CYAN << "Fitting model #" << iter << RESET;
    #endif

    double prevError = prevModel->measureError(sample);
    if (prevError == 0)
    {
      #ifdef DEBUG
      cout << RED << "-- stopping, fitting error already is zero." << RESET << endl;
      #endif
      break;
    }

    // Explore next best parameters
    // TAOTOREVIEW: Add prev explored paths as taboo
    auto newModelWithError = generateNextBestModel(prevModel, sample);

    BaseFittedModel* newModel = get<0>(newModelWithError);
    double error = get<1>(newModelWithError);
    
    double errorDiff;
    if (error == 0)
    {
      #ifdef DEBUG
      cout << CYAN << ": fittng error hits zero" << RESET << endl;
      #endif
      break;
    }
    else if (error == prevError)
    {
      errorDiff = 0;
    }
    else (error - prevError)/min(error, prevError);

    #ifdef DEBUG
    cout << endl; // TAOTODO: Report error here
    #endif

    // Destroy prev model
    delete prevModel;

    iter++;
    prevModel = newModel;
  };

  #ifdef DEBUG
  cout << RED << "[Model fitting finished]" << RESET << endl;
  #endif

  return prevModel;
}
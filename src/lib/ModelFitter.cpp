#include "ModelFitter.h"

BaseFittedModel& ModelFitter::initModel(const FittingCriteria& crit) const
{

}

BaseFittedModel& ModelFitter::fit(const Mat& sample, const FittingCriteria& crit) const 
{
  double errorDiff = numeric_limits<double>::max();
  int iter = 0;
  
  #ifdef DEBUG
  cout << GREEN << "[Model fitting started]" << RESET << endl;
  #endif

  while (iter < crit.numMaxIter && errorDiff > crit.eps)
  {
    // TAOTODO:
    #ifdef DEBUG
    cout << CYAN << "Fitting model #" << iter << RESET << endl;
    #endif

    

    iter++;
  };
}
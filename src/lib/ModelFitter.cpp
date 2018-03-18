#include "ModelFitter.h"

tuple<BaseFittedModel*, double> ModelFitter::generateNextBestModel(BaseFittedModel* model, const Mat& sample) const
{
  vector<SearchWith> actions = {SCALING, TRANSLATION, RESHAPING, REAPPEARANCING};
  vector<tuple<BaseFittedModel*, double>> candidates;

  // Generate action params
  double scales[] = {1.01, 0.99};
  Point2d trans[] = {Point2d(-1,0), Point2d(0,-1), Point2d(1,0), Point2d(0,1)};
  Mat* smat       = pcaShape.permutationOfParams();
  Mat* amat       = pcaAppearance.permutationOfParams();

  for (auto a : actions)
  {
    // TAOTOREVIEW: better to use priority queue here?
    vector<function<BaseFittedModel* (BaseFittedModel*)>> operators;
    
    // TAOTODO:
    switch (a)
    {
      case SCALING:
        for (auto s : scales) 
        {
          function<BaseFittedModel* (BaseFittedModel*)> f = [](BaseFittedModel* m)
          { 
            m->clone()->setScale(s * m->scale); 
          };
          operators.push_back(f);
        }
        break;
      case TRANSLATION:
      case RESHAPING:
      case REAPPEARANCING:
        break;
    }

    // Apply the actions and pick the best model
    // 
  }

  // Identify the best model
  double bestError = get<1>(candidates.front());
  BaseFittedModel* bestCandidate = get<0>(candidates.front());
  for (auto c : candidates)
  {
    double e = get<1>(c);
    if (e <= bestError)
    {
      bestError = e;
      bestCandidate = get<0>(c);
    }
  }
  
  // TAOTODO: Delete other unselected models, to prevent mem leakage


  return bestCandidate;
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
    auto errorStr = fmt::format("{0:2d} %", errorDiff/100);
    cout << ", error diff : " << errorStr << endl;
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
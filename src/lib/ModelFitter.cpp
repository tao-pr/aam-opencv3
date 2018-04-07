#include "ModelFitter.h"

tuple<BaseFittedModel*, double> ModelFitter::generateNextBestModel(BaseFittedModel* model, const Mat& sample) const
{
  vector<SearchWith> actions = {SCALING, TRANSLATION, RESHAPING, REAPPEARANCING};
  vector<BaseFittedModel*> candidates;

  #ifdef DEBUG
  cout << CYAN << "Fitter : Generating next best model" << RESET << endl;
  #endif

  // Generate action params
  auto pcaShape      = aamPCA->getShapePCA();
  auto pcaAppearance = aamPCA->getAppearancePCA();
  double scales[]    = {1.01, 0.99};
  Point2d trans[]    = {Point2d(-1,0), Point2d(0,-1), Point2d(1,0), Point2d(0,1)};
  auto smat          = pcaShape.permutationOfParams();
  auto amat          = pcaAppearance.permutationOfParams();

  for (auto a : actions)
  {
    // TAOTOREVIEW: better to use priority queue here?
    
    switch (a)
    {
      case SCALING:
        for (auto s : scales) 
        {
          candidates.push_back(model->clone()->setScale(s * model->scale));
        }
        break;

      case TRANSLATION:
        for (auto t : trans)
        {
          candidates.push_back(model->clone()->setOrigin(model->origin + t));
        }
        break;

      case RESHAPING:
        /// TAOTODO: Memory issue here
        while (!smat.empty())
        {
          cout << "reshaping ..." << smat.size() << "left" << endl; // TAODEBUG:
          cout << "shapeParam ~ " << model->shapeParam.size() << endl;
          cout << "param add  ~ " << smat.top().size() << endl;
          Mat p = smat.top();
          candidates.push_back(model->clone()->setShapeParam(model->shapeParam + p));
          smat.pop();
        }
        break;

      case REAPPEARANCING:
        while (!amat.empty())
        {
          cout << "reappearancing ..." << endl; // TAODEBUG:
          candidates.push_back(model->clone()->setAppearanceParam(model->appearanceParam + amat.top()));
          amat.pop();
        }
        break;
    }
  }

  #ifdef DEBUG
  cout << "Candidates size : " << candidates.size() << endl;
  #endif

  // Identify the best model
  double bestError = numeric_limits<double>::max();
  BaseFittedModel* bestCandidate = candidates.front();
  for (auto c : candidates)
  {
    double e = c->measureError(sample);
    if (e <= bestError)
    {
      bestError = e;
      bestCandidate = c->clone();
    }
  }

  // Free the allocated blocks
  #ifdef DEBUG
  cout << "Deallocating candidates ..." << endl;
  #endif 
  for (auto p : candidates)
    delete p;

  return bestCandidate;
}

BaseFittedModel* ModelFitter::fit(const BaseFittedModel* initModel, const Mat& sample, const FittingCriteria& crit) const 
{
  double errorDiff = numeric_limits<double>::max();
  int iter = 0;

  // Start with the given initial model
  auto prevModel = initModel->clone();
  prevModel->setOrigin(crit.initPos);
  prevModel->setScale(crit.initScale);

  #ifdef DEBUG
  cout << GREEN << "[Model fitting started]" << RESET << endl;
  cout << "[Init model]" << endl;
  cout << *prevModel << endl;
  #endif


  // Adjust model parameters until converges
  while (iter < crit.numMaxIter && errorDiff > crit.eps)
  {
    #ifdef DEBUG
    cout << CYAN << "Fitting model #" << iter << RESET << endl;
    #endif

    double prevError = prevModel->measureError(sample);
    if (prevError == 0)
    {
      #ifdef DEBUG
      cout << RED << "-- stopping, fitting error already is zero." << RESET << endl;
      #endif
      break;
    }

    #ifdef DEBUG
    cout << YELLOW << "... Error so far ~ " << prevError << RESET << endl;
    #endif

    // Explore next best parameters
    // TAOTOREVIEW: Add prev explored paths as taboo
    auto newModelWithError = generateNextBestModel(prevModel, sample);

    BaseFittedModel* newModel = get<0>(newModelWithError);
    double error = get<1>(newModelWithError);
    
    double errorDiff;
    if (error == 0)
    {
      #ifdef DEBUG
      cout << CYAN << "... Fittng error hits zero" << RESET << endl;
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
    cout << "... Error diff : " << errorStr << endl;
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
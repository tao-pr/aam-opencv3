#include "ModelFitter.h"

ostream &operator<<(ostream &os, SearchWith const &s)
{
  string str;
  switch (s)
  {
    case SCALING: str = "SCALING"; break;
    case TRANSLATION: str = "TRANSLATION"; break;
    case RESHAPING: str = "RESHAPING"; break;
    case REAPPEARANCING: str = "REAPP"; break;
  }
  return os << str;
}

unique_ptr<BaseFittedModel> ModelFitter::generateNextBestModel(unique_ptr<BaseFittedModel> const& model, const Mat& sample, double* bestError, SearchWith* action) const
{
  vector<SearchWith> actions = {SCALING, TRANSLATION, RESHAPING, REAPPEARANCING};
  vector<unique_ptr<BaseFittedModel>> candidates;
  vector<SearchWith> candidateActions;

  #ifdef DEBUG
  cout << CYAN << "Fitter : Generating next best model" << RESET << endl;
  #endif

  // Generate action params
  auto pcaShape      = aamPCA->getShapePCA();
  auto pcaAppearance = aamPCA->getAppearancePCA();
  double scales[]    = {1.01, 0.99, 1.5, 0.5, 1.33, 0.67};
  Point2d trans[]    = {Point2d(-1,0), Point2d(0,-1), Point2d(1,0), Point2d(0,1),
                        Point2d(-5,0), Point2d(0,-5), Point2d(5,0), Point2d(0,5)};
  auto smat          = pcaShape.permutationOfParams();
  auto amat          = pcaAppearance.permutationOfParams();

  for (auto& a : actions)
  {
    switch (a)
    {
      case SCALING:
        for (auto& s : scales) 
        {
          auto ptrModel = model->clone();
          ptrModel->setScale(s * model->scale);
          candidates.push_back(move(ptrModel));
          candidateActions.push_back(a);
        }
        break;

      case TRANSLATION:
        for (auto& t : trans)
        {
          auto ptrModel = model->clone();
          ptrModel->setOrigin(model->origin + t);
          candidates.push_back(move(ptrModel));
          candidateActions.push_back(a);
        }
        break;

      case RESHAPING:
        for (auto& param : smat)
        {
          auto ptrModel = model->clone();
          ptrModel->setShapeParam(model->shapeParam + *param);
          candidates.push_back(move(ptrModel));
          candidateActions.push_back(a);
        }
        break;

      case REAPPEARANCING:
        for (auto& param : amat)
        {
          auto ptrModel = model->clone();
          ptrModel->setAppearanceParam(model->appearanceParam + *param);
          candidates.push_back(move(ptrModel));
          candidateActions.push_back(a);
        }
        break;
    }
  }

  #ifdef DEBUG
  cout << "Candidates size : " << candidates.size() << endl;
  #endif

  // Identify the best model
  *bestError = numeric_limits<double>::max();
  int bestId = 0;

  int i = 0;
  for (auto& c : candidates)
  {
    #ifdef DEBUG
    cout << YELLOW << "Assessing candidate ..." << RESET << endl;
    #endif

    double e = c->measureError(sample);
    if (e <= *bestError)
    {
      *bestError = e;
      bestId = i;
    }
    ++i;
  }

  if (i > 0)
  {
    auto p = candidates[bestId]->clone();
    #ifdef DEBUG
    cout << "best candidate index = " << bestId << endl;
    #endif
    
    if (action)
    {
      *action = candidateActions[bestId];
    }
    return move(p);
  }
  else return nullptr;
}

unique_ptr<BaseFittedModel> ModelFitter::fit(unique_ptr<BaseFittedModel>& initModel, const Mat& sample, const FittingCriteria& crit) const 
{
  double errorDiff = numeric_limits<double>::max();
  int iter = 0;
  double prevError;
  vector<SearchWith> recordedActions;

  vector<unique_ptr<BaseFittedModel>> prevModels;

  // Start with the given initial model
  prevModels.push_back(move(initModel->clone()));
  prevModels.back()->setOrigin(crit.initPos);
  prevModels.back()->setScale(crit.initScale);

  #ifdef DEBUG
  cout << GREEN << "[Model fitting started]" << RESET << endl;
  cout << "[Init model]" << endl;
  cout << *prevModels.back() << endl;
  #endif

  prevError = prevModels.back()->measureError(sample);

  // Adjust model parameters until converges
  while (iter < crit.numMaxIter && errorDiff > crit.eps)
  {
    #ifdef DEBUG
    cout << CYAN << "Fitting model #" << iter << RESET << endl;
    #endif

    if (prevError == 0)
    {
      #ifdef DEBUG
      cout << RED << "-- stopping, fitting error already is zero." << RESET << endl;
      #endif
      break;
    }

    #ifdef DEBUG
    cout << YELLOW << "... Error so far : " << prevError << RESET << endl;
    #endif

    // Explore next best parameters
    // TAOTOREVIEW: Add prev explored paths as taboo
    double error;
    auto& prevModel = prevModels.back();
    SearchWith action;
    auto newModel = generateNextBestModel(prevModel, sample, &error, &action);

    // TAOTODO: Restore the previous model if no actions could reduce the error

    recordedActions.push_back(action);

    if (error == 0)
    {
      #ifdef DEBUG
      cout << CYAN << "... Fitting error approaches ZERO" << RESET << endl;
      #endif
      prevModels.push_back(move(newModel));
      prevError = 0;
      break;
    }
    else if (error == prevError)
    {
      errorDiff = 0;
    }
    else errorDiff = (error - prevError)/max(error, prevError);

    #ifdef DEBUG
    auto errorStr = fmt::format("{:2f} %", errorDiff/100);
    cout << RED << "... Error diff : " << errorStr << RESET << endl;
    cout << YELLOW << "... Last Error : " << prevError << RESET << endl;
    cout << YELLOW << "... Error so far : " << error << RESET << endl;
    #endif

    iter++;
    prevModels.push_back(move(newModel));
    prevError = error;
  };

  #ifdef DEBUG
  cout << GREEN << "[Model fitting finished]" << endl;
  cout << "... " << iter << " iteration(s)" << endl;
  cout << "... best error : " << prevError << endl;
  cout << "... actions : ";
  for (auto& a : recordedActions)
  {
    cout << a << " -> ";
  }
  cout << "end" << endl;
  cout << *prevModels.back() << RESET << endl;
  #endif

  auto selectedModel = move(prevModels.back());
  return selectedModel;
}
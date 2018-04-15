#include "ModelFitter.h"

unique_ptr<BaseFittedModel> ModelFitter::generateNextBestModel(unique_ptr<BaseFittedModel> const& model, const Mat& sample, double* bestError) const
{
  vector<SearchWith> actions = {SCALING, TRANSLATION, RESHAPING, REAPPEARANCING};
  vector<unique_ptr<BaseFittedModel>> candidates;

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
        }
        break;

      case TRANSLATION:
        for (auto& t : trans)
        {
          auto ptrModel = model->clone();
          ptrModel->setOrigin(model->origin + t);
          candidates.push_back(move(ptrModel));
        }
        break;

      case RESHAPING:
        for (auto& param : smat)
        {
          auto ptrModel = model->clone();
          ptrModel->setShapeParam(model->shapeParam + *param);
          candidates.push_back(move(ptrModel));
        }
        break;

      case REAPPEARANCING:
        for (auto& param : amat)
        {
          auto ptrModel = model->clone();
          ptrModel->setAppearanceParam(model->appearanceParam + *param);
          candidates.push_back(move(ptrModel));
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
    return move(p);
  }
  else return nullptr;
}

unique_ptr<BaseFittedModel> ModelFitter::fit(unique_ptr<BaseFittedModel>& initModel, const Mat& sample, const FittingCriteria& crit) const 
{
  double errorDiff = numeric_limits<double>::max();
  int iter = 0;
  double prevError;

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
    auto newModel = generateNextBestModel(prevModel, sample, &error);
    
    if (error == 0)
    {
      #ifdef DEBUG
      cout << CYAN << "... Fitting error approaches ZERO" << RESET << endl;
      #endif
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
  cout << *prevModels.back() << RESET << endl;
  #endif

  auto selectedModel = move(prevModels.back());
  // prevModels.erase(prevModels.end());
  return selectedModel;
}
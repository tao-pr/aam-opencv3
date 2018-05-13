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

unique_ptr<ModelList> ModelFitter::generateNextBestModels(double prevError, unique_ptr<BaseFittedModel> const& model, const Mat& sample, int numModels) const
{
  vector<SearchWith> actions = {SCALING, TRANSLATION, RESHAPING, REAPPEARANCING};
  //vector<unique_ptr<BaseFittedModel>> candidates;
  //vector<SearchWith> candidateActions;

  unique_ptr<ModelList> outputs{new ModelList()};

  #ifdef DEBUG
  cout << CYAN << "Fitter : Generating next best " << numModels << " models" << RESET << endl;
  #endif

  // Generate action params
  auto pcaShape      = aamPCA->getShapePCA();
  auto pcaAppearance = aamPCA->getAppearancePCA();
  double scales[]    = {1.01, 0.99, 
                        1.5, 0.5, 
                        1.33, 0.67,
                        2.5, 0.4};
  Point2d trans[]    = {//Point2d(-1,0), Point2d(0,-1), Point2d(1,0), Point2d(0,1),
                        Point2d(-5,0), Point2d(0,-5), Point2d(5,0), Point2d(0,5),
                        Point2d(-10,0), Point2d(0,-10), Point2d(10,0), Point2d(0,10),
                        Point2d(-25,0), Point2d(0,-25), Point2d(25,0), Point2d(0,25)};
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
          outputs->push(ptrModel, ptrModel->measureError(sample));
          //candidates.push_back(move(ptrModel));
          //candidateActions.push_back(a);
        }
        break;

      case TRANSLATION:
        for (auto& t : trans)
        {
          auto ptrModel = model->clone();
          ptrModel->setOrigin(model->origin + t);
          outputs->push(ptrModel, ptrModel->measureError(sample));
          // candidates.push_back(move(ptrModel));
          // candidateActions.push_back(a);
        }
        break;

      case RESHAPING:
        for (auto param : smat)
        {
          auto ptrModel = model->clone();
          ptrModel->setShapeParam(model->shapeParam + *param);
          outputs->push(ptrModel, ptrModel->measureError(sample));
          // candidates.push_back(move(ptrModel));
          // candidateActions.push_back(a);
        }
        smat.clear();
        break;

      case REAPPEARANCING:
        for (auto param : amat)
        {
          auto ptrModel = model->clone();
          ptrModel->setAppearanceParam(model->appearanceParam + *param);
          outputs->push(ptrModel, ptrModel->measureError(sample));
          // candidates.push_back(move(ptrModel));
          // candidateActions.push_back(a);
        }
        amat.clear();
        break;
    }
  }

  #ifdef DEBUG
  cout << "Candidates size : " << candidates.size() << endl;
  #endif

  // for (auto& c : candidates)
  // {
  //   #ifdef DEBUG
  //   cout << YELLOW << "Assessing candidate ..." << RESET << endl;
  //   #endif

  //   double e = c->measureError(sample);
  //   outputs->push(c, e);
  // }

  return move(outputs->take(numModels));
}

unique_ptr<BaseFittedModel> ModelFitter::fit(unique_ptr<BaseFittedModel>& initModel, const Mat& sample, const FittingCriteria& crit) const 
{
  double errorDiff = numeric_limits<double>::max();
  int iter = 0;
  double prevError;

  ModelList models;

  // Start with the given initial model
  prevError = initModel->measureError(sample);
  models.push(initModel->clone(), prevError);
  models.ptr->setOrigin(crit.initPos);
  models.ptr->setScale(crit.initScale);

  #ifdef DEBUG
  cout << GREEN << "[Model fitting started]" << RESET << endl;
  cout << "[Init model]" << endl;
  cout << *models.ptr << endl;
  #endif

  // TAOTOREVIEW: Use prune ratio

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

    SearchWith action;

    // Generate top best K models
    // and keep all of them
    // - Each iter:
    //    + Generate new models from these previously kept models
    //    + Sort models by errors, remove bottom models each iter
    //    + Repeat until converge

    // TAOTODO: Iterate throught [models] and generateNextBestModels from each of them
    
        
    const int numModelsToGenerate = 8;
    auto newModels = generateNextBestModels(
      prevError, 
      prevModel, 
      sample, 
      numModelsToGenerate);

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
    else if (isinf(prevError))
    {
      errorDiff = 1;
    }
    else errorDiff = (prevError - error)/prevError;

    #ifdef DEBUG
    cout << RED << "... Error diff : " << errorDiff << RESET << endl;
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
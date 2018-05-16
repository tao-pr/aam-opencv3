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

ostream &operator<<(ostream &os, FittingCriteria const &c)
{
  return os << "Fitting criteria :" << endl
    << "...num Max iter = " << c.numMaxIter << endl
    << "...max Tree size = " << c.maxTreeSize << endl 
    << "...num models to gen = " << c.numModelsToGeneratePerIter << endl
    << "...min Error = " << c.minError << endl
    << "...init scale = " << c.initScale << endl
    << "...init pos = " << c.initPos << endl;
}

void ModelFitter::generateNextBestModels(
  unique_ptr<ModelList>& container, 
  double prevError, 
  unique_ptr<BaseFittedModel>& model, 
  const Mat& sample, 
  int numModels) const
{
  vector<SearchWith> actions = {SCALING, TRANSLATION, RESHAPING, REAPPEARANCING};

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
  Point2d trans[]    = {Point2d(-5,0), Point2d(0,-5), Point2d(5,0), Point2d(0,5),
                        Point2d(-10,0), Point2d(0,-10), Point2d(10,0), Point2d(0,10),
                        Point2d(-25,0), Point2d(0,-25), Point2d(25,0), Point2d(0,25)};
  auto smat          = pcaShape.permutationOfParams();
  auto amat          = pcaAppearance.permutationOfParams();

  #ifdef DEBUG
  cout << "Actions prepared, generating models ..." << endl;
  #endif

  for (auto& a : actions)
  {
    switch (a)
    {
      case SCALING:
        for (auto& s : scales) 
        {
          cout << "CLONING..." << endl; // TAODEBUG:
          auto ptrModel = model->clone();
          cout << "SCALING..." << endl; // TAODEBUG:
          ptrModel->setScale(s * model->scale);
          cout << "PUSHING..." << endl; // TAODEBUG:
          container->push(ptrModel, ptrModel->measureError(sample));
        }
        break;

      case TRANSLATION:
        for (auto& t : trans)
        {
          auto ptrModel = model->clone();
          ptrModel->setOrigin(model->origin + t);
          container->push(ptrModel, ptrModel->measureError(sample));
        }
        break;

      case RESHAPING:
        for (auto param : smat)
        {
          auto ptrModel = model->clone();
          ptrModel->setShapeParam(model->shapeParam + *param);
          container->push(ptrModel, ptrModel->measureError(sample));
        }
        smat.clear();
        break;

      case REAPPEARANCING:
        for (auto param : amat)
        {
          auto ptrModel = model->clone();
          ptrModel->setAppearanceParam(model->appearanceParam + *param);
          container->push(ptrModel, ptrModel->measureError(sample));
        }
        amat.clear();
        break;
    }
  }

  container->take(numModels);
}

unique_ptr<BaseFittedModel> ModelFitter::fit(unique_ptr<BaseFittedModel>& initModel, const Mat& sample, const FittingCriteria& crit) const 
{
  double errorDiff = numeric_limits<double>::max();
  int iter = 0;
  double prevError;

  ModelList models;

  // Start with the given initial model
  prevError = initModel->measureError(sample);
  auto cloneInitModel = initModel->clone();
  models.push(cloneInitModel, prevError);
  models.ptr->setOrigin(crit.initPos);
  models.ptr->setScale(crit.initScale);

  #ifdef DEBUG
  cout << GREEN << "[Model fitting started]" << RESET << endl;
  cout << crit << endl;
  cout << "[Init model]" << endl;
  cout << *models.ptr << endl;
  #endif

  // Adjust model parameters until converges
  while (iter < crit.numMaxIter)
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

    // Iterate through existing best [models] 
    // and generate next models from them, sorted by fitting error
    unique_ptr<ModelList> iterOutputs{ new ModelList() };
    ModelList* p = &models;

    // TAODEBUG:
    cout << "num models so far : " << models.size() << endl;

    while (p != nullptr)
    {
      generateNextBestModels(
        iterOutputs,
        p->v, 
        p->ptr,
        sample,
        crit.numModelsToGeneratePerIter);  

      // TAOTODO: 2nd iter will break
      if (p->next != nullptr && p->next->ptr != nullptr)
        p = p->next.get();
      else   
        break;
    }

    // Take best K models
    p = iterOutputs.get();
    while (true)
    {
      models.push(p->ptr, p->v);

      if (p->next != nullptr)
        p = p->next.get();
      else
        break;
    }

    models.take(crit.maxTreeSize);

    #ifdef DEBUG
    cout << "-----------------------------" << endl;
    cout << "[Model Iter #" << iter << "]" << endl;
    cout << "... Best error so far   : " << models.v << endl;
    cout << "... Best error new iter : " << iterOutputs->v << endl;
    cout << "... Tree size : " << models.size() << endl;
    #endif

    iter++;
  };

  return move(models.ptr);
}
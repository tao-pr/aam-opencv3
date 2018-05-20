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

void ModelFitter::iterateModelExpansion(ModelList* const modelPtr)
{
  #ifdef DEBUG
  cout << CYAN << "Fitter : Generating next best models." << RESET << endl;
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
          auto ptrModel = modelPtr->ptr->clone();
          ptrModel->setScale(s * modelPtr->ptr->scale);
          buffer.push(ptrModel, ptrModel->measureError(sample));
        }
        break;

      case TRANSLATION:
        for (auto& t : trans)
        {
          auto ptrModel = modelPtr->ptr->clone();
          ptrModel->setOrigin(modelPtr->ptr->origin + t);
          buffer.push(ptrModel, ptrModel->measureError(sample));
        }
        break;

      case RESHAPING:
        for (auto param : smat)
        {
          auto ptrModel = modelPtr->ptr->clone();
          ptrModel->setShapeParam(modelPtr->ptr->shapeParam + *param);
          buffer.push(ptrModel, ptrModel->measureError(sample));
        }
        smat.clear();
        break;

      case REAPPEARANCING:
        for (auto param : amat)
        {
          auto ptrModel = modelPtr->ptr->clone();
          ptrModel->setAppearanceParam(modelPtr->ptr->appearanceParam + *param);
          buffer.push(ptrModel, ptrModel->measureError(sample));
        }
        amat.clear();
        break;
    }
  }

  // Repeat until the model pointer reaches the end
  if (modelPtr->next != nullptr && modelPtr->next->ptr != nullptr)
    iterateModelExpansion(modelPtr->next.get());
}

unique_ptr<BaseFittedModel> ModelFitter::fit(unique_ptr<BaseFittedModel>& initModel, const Mat& sample, const FittingCriteria& crit)
{
  double errorDiff = numeric_limits<double>::max();
  double prevError;

  // Clear all previous fitting states
  #ifdef DEBUG
  cout << "Initialising fitting states." << endl;
  #endif
  this->models.clear();
  this->buffer.clear();

  // Start with the given initial model
  prevError = initModel->measureError(sample);
  auto cloneInitModel = initModel->clone();
  buffer.push(cloneInitModel, prevError);
  buffer.ptr->setOrigin(crit.initPos);
  buffer.ptr->setScale(crit.initScale);

  #ifdef DEBUG
  cout << GREEN << "[Model fitting started]" << RESET << endl;
  cout << crit << endl;
  cout << "[Init model]" << endl;
  cout << *buffer.ptr << endl;
  #endif

  // Adjust model parameters until converges
  int iter = 0;
  while (iter < crit.numMaxIter)
  {
    #ifdef DEBUG
    cout << CYAN << "Fitting model #" << iter << RESET << endl;
    cout << YELLOW << "... Error so far : " << prevError << RESET << endl;
    #endif

    this->buffer.clear();

    // TAODEBUG:
    cout << "num models so far : " << models.size() << endl;
    models.printValueList("Errors : ");

    iterateModelExpansion(&this->models);

    #ifdef DEBUG
    cout << "num generated models : " << buffer.size() << endl;
    #endif

    break; // TAODEBUG:

    models.take(crit.maxTreeSize);

    #ifdef DEBUG
    cout << "-----------------------------" << endl;
    cout << CYAN << "[Model Iter #" << iter << "]" << RESET << endl;
    cout << "... Best error so far   : " << models.v << endl;
    cout << "... Best error new iter : " << buffer.v << endl;
    cout << "... Tree size : " << models.size() << endl;
    #endif

    // TAOTODO: Take best K buffered models into [models]

    iter++;
  };

  return move(models.ptr);
}
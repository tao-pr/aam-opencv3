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
    << "...min err diff = " << c.minErrorImprovement << endl
    << "...init scale = " << c.initScale << endl
    << "...init pos = " << c.initPos << endl;
}

void ModelFitter::iterateModelExpansion(
  ModelList* const modelPtr,
  SearchWith action,
  double scale)
{
  #ifdef DEBUG
  string actionStr = "";
  switch (action)
  {
    case TRANSLATION: actionStr = "translation"; break;
    case SCALING:     actionStr = "scaling"; break;
    case RESHAPING:   actionStr = "reshaping"; break;
    case REAPPEARANCING: actionStr = "reappearancing"; break;
  }
  cout << "... Generating new models with " << actionStr << endl;
  #endif

  assert(modelPtr != nullptr);
  assert(modelPtr->ptr != nullptr);

  // Generate action params
  auto pcaShape      = aamPCA->getShapePCA();
  auto pcaAppearance = aamPCA->getAppearancePCA();
  double scales[]    = {1.01, 0.99, 1.0, 1.5, 0.5};
  Point2d trans[]    = {Point2d(-1,0), Point2d(0,-1), 
                        Point2d(1,0), Point2d(0,1),
                        Point2d(1,1), Point2d(1,-1),
                        Point2d(-1,1), Point2d(-1,-1),
                        Point2d(0,0)
                      };
  
  int smatSize = pcaShape.getSizeOfPermutationOfParams();
  int amatSize = pcaAppearance.getSizeOfPermutationOfParams();
  Mat *smat = new Mat[smatSize]; 
  Mat *amat = new Mat[amatSize];
  for (int i=0; i<smatSize; i++) smat[i] = Mat::zeros(1, pcaShape.dimension(), CV_64FC1);
  for (int i=0; i<amatSize; i++) amat[i] = Mat::zeros(1, pcaAppearance.dimension(), CV_64FC1);
  pcaShape.permutationOfParams(smat);
  pcaAppearance.permutationOfParams(amat);

  // TAOTODO: Apply step size and decreasing it iteratively

  // TAOTODO: Reject if shape param or app param is ZERO matrix
  switch (action)
  {
    case SCALING:
      for (auto& s : scales) 
      {
        TRY
        auto ptrModel = modelPtr->ptr->clone();
        double newScale = s * modelPtr->ptr->scale * scale;
        if (newScale > 0)
        {
          ptrModel->setScale(newScale);
          double e = ptrModel->measureError(sample);
          buffer.push(ptrModel, e);  
        }
        END_TRY
      }
      break;

    case TRANSLATION:
      for (auto& t : trans)
      {
        TRY
        auto ptrModel = modelPtr->ptr->clone();
        auto newOrigin = modelPtr->ptr->origin + t * scale;
        if (newOrigin.x >= 0 && newOrigin.y >= 0)
        {
          ptrModel->setOrigin(newOrigin);
          double e = ptrModel->measureError(sample);
          buffer.push(ptrModel, e);
        }
        END_TRY
      }
      break;

    case RESHAPING:
      for (int i=0; i<smatSize; i++)
      {
        TRY
        auto ptrModel = modelPtr->ptr->clone();
        Mat param = modelPtr->ptr->shapeParam * scale + smat[i];
        ptrModel->setShapeParam(param);
        double e = ptrModel->measureError(sample);
        buffer.push(ptrModel, e);
        END_TRY
      }
      break;

    case REAPPEARANCING:
      for (int i=0; i<amatSize; i++)
      {
        TRY
        auto ptrModel = modelPtr->ptr->clone();
        Mat param = modelPtr->ptr->appearanceParam * scale + amat[i];
        ptrModel->setAppearanceParam(param);
        double e = ptrModel->measureError(sample);
        buffer.push(ptrModel, e);
        END_TRY
      }
      break;
  }

  delete[] smat;
  delete[] amat;

  // Repeat until the model pointer reaches the end
  if (modelPtr->next != nullptr && modelPtr->next->ptr != nullptr)
    iterateModelExpansion(modelPtr->next.get(), action, scale);
}

void ModelFitter::transferFromBuffer(int nLeft)
{
  if (nLeft <= 0 || buffer.ptr == nullptr) return;
  models.push(buffer.ptr, buffer.v);
  if (buffer.next != nullptr)
  {
    // Replace self with next element
    buffer.ptr = move(buffer.next->ptr);
    buffer.v = buffer.next->v;
    if (buffer.next->next != nullptr)
      buffer.next = move(buffer.next->next);
    else 
      buffer.next = nullptr;

    transferFromBuffer(nLeft-1);
  }
}

unique_ptr<BaseFittedModel> ModelFitter::fit(unique_ptr<BaseFittedModel>& initModel)
{
  assert(initModel != nullptr);
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
  int iter = 0;
  double scale = 1;
  deque<SearchWith> ACTIONS = {TRANSLATION, SCALING, RESHAPING, REAPPEARANCING};
  
  while (iter < crit.numMaxIter)
  {
    #ifdef DEBUG
    cout << CYAN << "Fitting model #" << iter << RESET << endl;
    cout << YELLOW << "... Best error so far : " << models.v << RESET << endl;
    #endif

    this->buffer.clear();

    #ifdef DEBUG
    cout << "... Num models so far : " << models.size() << endl;
    models.printValueList("... Errors : ");
    #endif

    iterateModelExpansion(&this->models, ACTIONS[0], scale);

    #ifdef DEBUG
    cout << "... New models generated : " << min(buffer.size(), crit.numModelsToGeneratePerIter) << endl;
    cout << "... Best error this iter : " << buffer.v << endl;
    #endif

    double bestPrevError = models.v;
    double bestNewError = buffer.v;

    // Take best K buffered models into [models]
    transferFromBuffer(crit.numModelsToGeneratePerIter);
    models.take(crit.maxTreeSize);

    #ifdef DEBUG
    cout << "... Tree size : " << models.size() << endl;
    #endif

    if (bestPrevError - bestNewError < crit.minErrorImprovement)
    {
      // Shrink scale
      if (scale > 0.125)
      {
        scale *= 0.5;
        #ifdef DEBUG
        cout << "... Steady error, shrinking scale to " << scale << endl;
        #endif
      }
      else
      {
        // Iterate to the next action
        if (ACTIONS.size()>0)
        {
          #ifdef DEBUG
          cout << "... Steady error, iterate to next action" << endl;
          #endif
          ACTIONS.pop_front();
          scale = 1;
        }
        else break;
      }
    }

    iter++;
  };

  return move(models.ptr);
}
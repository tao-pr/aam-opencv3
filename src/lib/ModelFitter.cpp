#include "ModelFitter.h"

const double ModelFitter::scales[5] = {1.01, 0.99, 1.0, 1.5, 0.5};
const Point2d ModelFitter::trans[9] = {
  Point2d(-1,0), Point2d(0,-1), 
  Point2d(1,0), Point2d(0,1),
  Point2d(1,1), Point2d(1,-1),
  Point2d(-1,1), Point2d(-1,-1),
  Point2d(0,0)
};

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

void ModelFitter::buildCache()
{
  this->pcaShape      = aamPCA->getShapePCA();
  this->pcaAppearance = aamPCA->getAppearancePCA();
  this->smatSize = pcaShape.getSizeOfPermutationOfParams();
  this->amatSize = pcaAppearance.getSizeOfPermutationOfParams();
  this->smat = new Mat[this->smatSize];
  this->amat = new Mat[this->amatSize];
  for (int i=0; i<smatSize; i++) smat[i] = Mat::zeros(1, pcaShape.dimension(), CV_64FC1);
  for (int i=0; i<amatSize; i++) amat[i] = Mat::zeros(1, pcaAppearance.dimension(), CV_64FC1);
  pcaShape.permutationOfParams(smat);
  pcaAppearance.permutationOfParams(amat);
}

void ModelFitter::iterateModelExpansionParallel(
  ModelList* const modelPtr,
  SearchWith action,
  double scale)
{
  assert(modelPtr != nullptr);
  assert(modelPtr->ptr != nullptr);

  const int SKIP_SIZE = 2;
  #define IN_RANGE(v,_min,_max) (v>_min && v<_max)

  // TAOTODO: Parallelise this function

  while (modelPtr->next != nullptr && modelPtr->next->ptr != nullptr)
  {
    auto modl = modelPtr->next.get();
  }
  

  // Generate new model by varying the parameter
  // NOTE: A new model may be ignored if it does not produce smaller error than base minimum.
  switch (action)
  {
    case SCALING:
      for (auto& s : scales) 
      {
        TRY
        auto ptrModel = modelPtr->ptr->clone();
        double newScale = s * modelPtr->ptr->scale * scale;
        if (newScale > 0 && newScale >= crit.minScale 
          && newScale <= crit.maxScale
          && IN_RANGE(newScale, SCALING_MIN, SCALING_MAX))
        {
          ptrModel->setScale(newScale);
          double e = ptrModel->measureError(sample, SKIP_SIZE);
          double e0 = ptrModel->measureError(zero, SKIP_SIZE);
          if (e<e0) buffer.push(ptrModel, e);
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
        if (newOrigin.x >= 0 && newOrigin.y >= 0 
          && IN_RANGE(t.x * scale, TRANSLATION_MIN, TRANSLATION_MAX))
        {
          ptrModel->setOrigin(newOrigin);
          double e = ptrModel->measureError(sample, SKIP_SIZE);
          double e0 = ptrModel->measureError(zero, SKIP_SIZE);
          if (e<e0) buffer.push(ptrModel, e);
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
        double _mi, _mx;
        minMaxLoc(param, &_mi, &_mx);
        if (_mi > RESHAPING_MIN && _mx < RESHAPING_MAX)
        {
          ptrModel->setShapeParam(param);
          double e = ptrModel->measureError(sample, SKIP_SIZE);
          double e0 = ptrModel->measureError(zero, SKIP_SIZE);
          if (e<e0) buffer.push(ptrModel, e);
        }
        END_TRY
      }
      break;

    case REAPPEARANCING:
      for (int i=0; i<amatSize; i++)
      {
        TRY
        auto ptrModel = modelPtr->ptr->clone();
        Mat param = modelPtr->ptr->appearanceParam * scale + amat[i];
        double _mi, _mx;
        minMaxLoc(param, &_mi, &_mx);
        if (_mi > REAPPEARANCING_MIN && _mx < REAPPEARANCING_MAX)
        {
          ptrModel->setAppearanceParam(param);
          double e = ptrModel->measureError(sample, SKIP_SIZE);
          double e0 = ptrModel->measureError(zero, SKIP_SIZE);
          if (e<e0) buffer.push(ptrModel, e);
        }
        END_TRY
      }
      break;
  }

  // Repeat until the model pointer reaches the end
  if (modelPtr->next != nullptr && modelPtr->next->ptr != nullptr)
    iterateModelExpansion(modelPtr->next.get(), action, scale);
}

void ModelFitter::iterateModelExpansion(
  ModelList* const modelPtr,
  SearchWith action,
  double scale)
{
  assert(modelPtr != nullptr);
  assert(modelPtr->ptr != nullptr);

  const int SKIP_SIZE = 2;
  #define IN_RANGE(v,_min,_max) (v>_min && v<_max)

  // Generate new model by varying the parameter
  // NOTE: A new model may be ignored if it does not produce smaller error than base minimum.
  switch (action)
  {
    case SCALING:
      for (auto& s : scales) 
      {
        TRY
        auto ptrModel = modelPtr->ptr->clone();
        double newScale = s * modelPtr->ptr->scale * scale;
        if (newScale > 0 && newScale >= crit.minScale 
          && newScale <= crit.maxScale
          && IN_RANGE(newScale, SCALING_MIN, SCALING_MAX))
        {
          ptrModel->setScale(newScale);
          double e = ptrModel->measureError(sample, SKIP_SIZE);
          double e0 = ptrModel->measureError(zero, SKIP_SIZE);
          if (e<e0) buffer.push(ptrModel, e);
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
        if (newOrigin.x >= 0 && newOrigin.y >= 0 
          && IN_RANGE(t.x * scale, TRANSLATION_MIN, TRANSLATION_MAX))
        {
          ptrModel->setOrigin(newOrigin);
          double e = ptrModel->measureError(sample, SKIP_SIZE);
          double e0 = ptrModel->measureError(zero, SKIP_SIZE);
          if (e<e0) buffer.push(ptrModel, e);
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
        double _mi, _mx;
        minMaxLoc(param, &_mi, &_mx);
        if (_mi > RESHAPING_MIN && _mx < RESHAPING_MAX)
        {
          ptrModel->setShapeParam(param);
          double e = ptrModel->measureError(sample, SKIP_SIZE);
          double e0 = ptrModel->measureError(zero, SKIP_SIZE);
          if (e<e0) buffer.push(ptrModel, e);
        }
        END_TRY
      }
      break;

    case REAPPEARANCING:
      for (int i=0; i<amatSize; i++)
      {
        TRY
        auto ptrModel = modelPtr->ptr->clone();
        Mat param = modelPtr->ptr->appearanceParam * scale + amat[i];
        double _mi, _mx;
        minMaxLoc(param, &_mi, &_mx);
        if (_mi > REAPPEARANCING_MIN && _mx < REAPPEARANCING_MAX)
        {
          ptrModel->setAppearanceParam(param);
          double e = ptrModel->measureError(sample, SKIP_SIZE);
          double e0 = ptrModel->measureError(zero, SKIP_SIZE);
          if (e<e0) buffer.push(ptrModel, e);
        }
        END_TRY
      }
      break;
  }

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

unique_ptr<BaseFittedModel> ModelFitter::fit(unique_ptr<BaseFittedModel>& initModel, int skipPixels)
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
  prevError = initModel->measureError(sample, skipPixels);
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
  deque<SearchWith> ACTIONS = {
    TRANSLATION, SCALING, 
    TRANSLATION, SCALING, 
    RESHAPING, REAPPEARANCING
  };
  
  while (iter < crit.numMaxIter)
  {
    #ifdef DEBUG
    cout << CYAN << "Fitting model #" << iter << RESET << endl;
    cout << YELLOW << "... Best error so far : " << models.v << RESET << endl;
    #endif

    this->buffer.clear();

    #ifdef DEBUG
    models.printValueList("... Errors : ");
    string actionStr = "";
    switch (ACTIONS[0])
    {
      case TRANSLATION: actionStr = "translation"; break;
      case SCALING:     actionStr = "scaling"; break;
      case RESHAPING:   actionStr = "reshaping"; break;
      case REAPPEARANCING: actionStr = "reappearancing"; break;
    }
    cout << "... Generating new models with " << actionStr << endl;
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
        scale *= 0.67; // TAOTOREVIEW: Use different decay ratios as per action
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
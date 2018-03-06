#include "ModelCollection.h"

long long ModelCollection::generateUID()
{
  return static_cast<long long>(floor(time(NULL)));
}

ModelCollection::~ModelCollection()
{
  #ifdef DEBUG 
  auto cnt = fmt::format("({0} items)", this->items.size());
  cout << YELLOW << "Cleaning up ModelCollection @" << getUID() << cnt  << RESET << endl;
  #endif

  clear();

  #ifdef DEBUG
  cout << "... ModelCollection @" << getUID() << " cleared and destroyed." << endl;
  #endif
}

void ModelCollection::clear()
{
  #ifdef DEBUG
  cout << YELLOW << "BaseModelModelCollection::clear @" << getUID() << RESET << endl;
  #endif
  for (auto item : this->items)
  {
    delete item;
  }
}


BaseModel* ModelCollection::procrustesMean(double tol, int maxIter)
{
  #ifdef DEBUG
  cout << "BaseModelModelCollection::procrustesMean @" << getUID() << endl;
  #endif
  auto alignedSet  = this->clone();
  double lastError = 0;
  double tl        = numeric_limits<double>::max();
  int iter         = 0;

  auto tolerance = [&](double e, double e0)
  {
    return abs(e-e0)/min(e0,e);
  };

  #ifdef DEBUG
  cout << GREEN << "[Calculating Procrustes mean]" << RESET << endl;
  #endif

  while (tl > tol && iter < maxIter)
  {
    #ifdef DEBUG
    cout << CYAN << "... Aligning iter# " << RESET << iter << endl;
    cout << "... Normalising rotation" << endl;
    #endif

    alignedSet->normaliseRotation();
    auto mean  = alignedSet->items[0];
    
    #ifdef DEBUG
    cout << "... Calculating procrustes distance" << endl;
    #endif

    double err = alignedSet->sumProcrustesDistance(mean);

    #ifdef DEBUG
    cout << "... Error so far : " << err << endl;
    cout << "... tol : " << tolerance(err, lastError) << endl;
    #endif

    iter++;
    tl = tolerance(err, lastError);
    lastError = err;
  }
  
  #ifdef DEBUG
  cout << GREEN << "... [Alignment done]" << endl;
  #endif

  auto meanModel = alignedSet->items[0];

  swap(this->items, alignedSet->items);

  return meanModel;
}

double ModelCollection::sumProcrustesDistance(const BaseModel* targetModel) const
{
  double sumDist = 0.0;
  for (auto model : this->items)
  {
    sumDist += model->procrustesDistance(targetModel);
  }
  return sumDist;
}

void ModelCollection::normaliseRotation()
{
  cerr << RED << "ModelCollection::normaliseRotation is not implemented" << RESET << endl;
}

Mat ModelCollection::covariance(const BaseModel* mean) const
{
  int N = this->items[0]->getMat().rows;
  Mat cov = Mat::zeros(N, N, CV_64FC1);
  double M = 0;
  for (auto model : this->items)
  {
    auto res = model->getMat() - mean->getMat();
    cov = cov + res * res.t();
    M += 1.0;
  }
  return cov * (1/M);
}

/**
 * NOTE: In general, [maxDimension] argument is ignore for the generic model collection
 */
ModelPCA* ModelCollection::pca(const BaseModel* mean, int maxDimension) const
{
  #ifdef DEBUG
  cout << GREEN << "[Computing PCA]" << RESET << endl;
  #endif

  Mat meanVector = mean->toRowVector();
  Mat data       = this->toMat();

  #ifdef DEBUG
  cout << "... mean model size : " << meanVector.size() << endl;
  cout << "... data size       : " << data.size() << endl;
  #endif

  auto pca = PCA(data, meanVector, CV_PCA_DATA_AS_ROW);

  // Collect lambdas
  // TAOTOREVIEW: Take only highest K lambda where K<N
  
  #ifdef DEBUG
  cout << "... eigenvalues  : " << pca.eigenvalues.size() << endl;
  cout << "... eigenvectors : " << pca.eigenvectors.size() << endl;
  #endif

  // Create a Shape model PCA by default
  return new ShapeModelPCA(pca);
}
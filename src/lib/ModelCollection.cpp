#include "ModelCollection.h"

ModelCollection::~ModelCollection()
{
  if (this->verbose) 
  {
    auto cnt = fmt::format("({0} items)", this->items.size());
    cout << YELLOW << "Cleaning up ModelCollection " << cnt  << RESET << endl;
  }

  clear();

  if (this->verbose) cout << "... ModelCollection cleared and destroyed." << endl;
}

void ModelCollection::clear()
{
  for (auto item : this->items)
  {
    delete item;
  }
}

BaseModel* ModelCollection::procrustesMean(double tol, int maxIter)
{
  auto alignedSet  = this->clone();
  double lastError = 0;
  double tl        = numeric_limits<double>::max();
  int iter         = 0;

  auto tolerance = [&](double e, double e0)
  {
    return abs(e-e0)/min(e0,e);
  };

  if (verbose) cout << GREEN << "[Calculating Procrustes mean]" << RESET << endl;
  while (tl > tol && iter < maxIter)
  {
    auto mean  = alignedSet->items[0];
    if (verbose) cout << CYAN << "... Aligning iter# " << RESET << iter << endl;
    if (verbose) cout << "... Normalising rotation" << endl;
    alignedSet->normaliseRotation();
    if (verbose) cout << "... Calculating procrustes distance" << endl;
    double err = alignedSet->sumProcrustesDistance(mean);

    if (verbose) cout << "... Error so far : " << err << endl;
    if (verbose) cout << "... tol : " << tolerance(err, lastError) << endl;

    iter++;
    tl = tolerance(err, lastError);
    lastError = err;
  }
  if (verbose) cout << "... Alignment done" << endl;

  clear();
  swap(this->items, alignedSet->items);

  return this->items[0];// TAOREVIEW: Return proper mean object
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

ModelEncoder ModelCollection::pca(const BaseModel* mean) const
{
  if (verbose) cout << GREEN << "[Computing PCA]" << RESET << endl;
  Mat meanVector = mean->toRowVector();
  Mat data       = this->toMat();
  if (verbose) 
  {
    cout << "... mean model size : " << meanVector.rows << " x " << meanVector.cols << endl;
    cout << "... data size       : " << data.rows << " x " << data.cols << endl;
  }
  auto pca = PCA(data, meanVector, CV_PCA_DATA_AS_ROW);

  // Collect lambdas
  // TAOTOREVIEW: Take only highest K lambda where K<N
  int N = pca.eigenvalues.rows;
  if (verbose)
  {
    cout << "... eigenvalues  : " << N << endl;
    cout << "... eigenvectors : " << pca.eigenvectors.rows << " x " << pca.eigenvectors.cols << endl;
  }

  // Compose a shape param set from eigenvalues
  return ModelEncoder(mean->toColVector(), pca.eigenvectors);
}
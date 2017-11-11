#include "BaseCollection.h"

~ModelCollection::ModelCollection()
{
  if (this->verbose) 
  {
    auto cnt = fmt::format("({0} items)", this->items.size());
    cout << YELLOW << "Cleaning up ModelCollection " << cnt  << RESET << endl;
  }

  for (auto item : this->items)
  {
    delete item;
  }

  if (this->verbose) cout << "... ModelCollection cleared and destroyed." << endl;
}

ModelAlignment ModelCollection::procrustesMean(double tol=1e-3, int maxIter=10) const
{
  // Pick the first element from the collection as initial mean
  auto mean        = this->items[0];
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
    if (verbose) cout << CYAN << "... Aligning iter# " << RESET << iter << endl;
    alignedSet = alignedSet.clone(this->verbose).normaliseRotation();
    double err = alignedSet.sumProcrustesDistance(mean);

    if (verbose) cout << "... Error so far : " << err << endl;
    if (verbose) cout << "... tol : " << tolerance(err, lastError) << endl;

    iter++;
    tl = tolerance(err, lastError);
    lastError = err;
  }
  if (verbose) cout << "... Alignment done" << endl;

  // TAOTODO: Following would possibly cause memory leakage?
  return make_tuple(alignedSet, mean->clone());
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

Mat ModelCollection::covariance(const BaseModel* mean) const
{
  int shapeSize = this->items[0]->getMat().rows;
  Mat cov = Mat::zeros(shapeSize, shapeSize, CV_64FC1);
  double N = 0;
  for (auto model : this->items)
  {
    auto res = model->getMat() - mean->getMat();
    cov = cov + res * res.t();
    N += 1.0;
  }
  return cov * (1/N);
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
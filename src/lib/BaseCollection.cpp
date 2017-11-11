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
}

Alignment ModelCollection::procrustesMean(double tol=1e-3, int maxIter=10) const
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

  if (verbose) cout << GREEN << "[Procrustes Mean Model]" << RESET << endl;
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

  return Alignment(alignedSet, mean);
}

double ModelCollection::sumProcrustesDistance(const BaseModel* targetModel) const
{

}

Mat ModelCollection::covariance(const BaseModel* mean) const
{

}

ModelEncoder ModelCollection::pca(const BaseModel* mean) const
{

}
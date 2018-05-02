#include "TreeSearch.h"

TreeSearch::TreeSearch()
{
  this->model = nullptr;
  bestError = numeric_limits<double>::max();
  isOrphan = true;
}

TreeSearch::TreeSearch(const unique_ptr<BaseFittedModel> &_model, double error)
{
  this->model = move(_model);
  bestError = error;
  isOrphan = false;
}

unique_ptr<BaseFittedModel> TreeSearch::get() const
{
  return this->model;
}

bool TreeSearch::isTerminal() const
{
  return (branches.size()==0);
}

unique_ptr<TreeSearch> TreeSearch::expandBranches(unique_ptr<ModelFitter> fitter) const
{
  // TAOTODO:
}

void TreeSearch::prune(double decayRatio)
{

}
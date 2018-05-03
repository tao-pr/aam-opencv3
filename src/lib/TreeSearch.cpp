#include "TreeSearch.h"

TreeSearch::TreeSearch()
{
  this->model = nullptr;
  bestError = numeric_limits<double>::max();
  isOrphan = true;
}

TreeSearch::TreeSearch(unique_ptr<BaseFittedModel> _model, double error)
: model(move(_model))
{
  bestError = error;
  isOrphan = false;
}

bool TreeSearch::isTerminal() const
{
  return (branches.size()==0);
}

unique_ptr<TreeSearch> TreeSearch::expandBranches(unique_ptr<ModelFitter> fitter) const
{
  // TAOTODO:
}

bool TreeSearch::prune(double decayRatio)
{
  for (auto& b : this->branches)
  {
    bool toBeRemoved = b->prune(decayRatio);
  }
}

const int TreeSearch::getLevel() const
{
  if (this->branches.size()==0) return 1;
  else
  {
    int m = 0;
    for (auto& b : this->branches)
    {
      m = max(m, b->getLevel());
    }
    return m;
  }
}

void TreeSearch::deleteAllBranches()
{
  for (auto& b : this->branches)
  {
    b->deleteAllBranches();
  }

  this->branches.clear();
}
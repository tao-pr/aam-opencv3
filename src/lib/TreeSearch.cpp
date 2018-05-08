#include "TreeSearch.h"

TreeSearch::TreeSearch()
{
  this->model = nullptr;
  bestError = numeric_limits<double>::max();
  isOrphan = true;
}

TreeSearch::TreeSearch(unique_ptr<BaseFittedModel> _model, double error)
: model(move(_model)), bestError(error), error(error), isOrphan(false)
{
}

bool TreeSearch::isTerminal() const
{
  return (branches.size()==0);
}

unique_ptr<TreeSearch> TreeSearch::expandBranches(unique_ptr<ModelFitter> &fitter, const Mat& sample) const
{
  bool isLeave = true;
  int numExpanded = 0;
  double bestErrorAfterExpansion = this->error;

  for (auto& b : this->branches)
  {
    if (!b->isOrphan)
    {
      isLeave &= b->isTerminal();
      numExpanded ++;

      // Generate new child trees if this is a terminal node
      if (b->isTerminal())
      {
        double nextError;
        auto nextModels = fitter->generateNextBestModel(
          this->error,
          this->model,
          sample,
          &nextError);

        if (nextError < bestErrorAfterExpansion)
          bestErrorAfterExpansion = nextError;

        // TAOTODO: Assign <nextModels> -> this branches
      }
      else
      {
        b->expandBranches();
      }
    }
  }

  #ifdef DEBUG
  if (isLeave)
  {
    cout << "... Expanding leave tree (" + numExpanded + ")" << endl;
  }
  #endif
}

bool TreeSearch::prune(double decayRatio)
{
  for (auto& b : this->branches)
  {
    bool toBeRemoved = b->prune(decayRatio);
  }
}

const int TreeSearch::getDepth() const
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
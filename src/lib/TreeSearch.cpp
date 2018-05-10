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

double TreeSearch::searchForBestModel(Mat& sample, int maxDepth, double decayRatio)
{
  // TAOTODO:


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
        // TAOTODO: Generate all combinations as next models

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
  // Remove branches with top errors
  // and make their parents orphan
  for (auto& b : this->branches)
  {
    bool toBeRemoved = b->prune(decayRatio);

    // TAOTODO:
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
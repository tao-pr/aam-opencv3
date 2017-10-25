#include "AppearanceCollection.h"

Mat AppearanceCollection::toMat() const
{
  // Each element has to have equal boundary
  //
  Mat frontMat = this->items[0].toRowVector();
  int N = this->items.size();
  int M = frontMat.cols;
  auto type = frontMat.type();
  Mat m = Mat(N, M, type);
  int j = 0;
  for (auto item : this->items)
  {
    item.toRowVector().row(0).copyTo(m.row(j));
    ++j;
  }
  return m;
}

// TAOTODO:

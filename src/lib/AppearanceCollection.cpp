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

Mat AppearanceCollection::covariance(const Appearance& mean) const
{
  Mat frontMat = this->items[0].toRowVector();
  int M = frontMat.cols;
  int N = this->items.size();
  Mat cov = Mat::zeros(M, M, CV_32FC3);
  Mat meanVector = mean.toRowVector();
  for (auto item : this->items)
  {
    auto res = item.toRowVector() - meanVector;
    cov = cov + res * res.t();
  }
  return (1/(double)N) * cov;
}

AppearanceEncoder AppearanceCollection::pca(const Appearance& meanAppearance) const
{

}

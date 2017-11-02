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

/**
 * Compute eigenvectors of the covariance matrix of the appearnces
 */
ModelEncoder AppearanceCollection::pca(const Appearance& meanAppearance) const
{
  if (verbose) cout << GREEN << "[Computing Appearance PCA]" << RESET << endl;

  Mat meanVector = meanAppearance.toRowVector();
  Mat data       = this->toMat();
  if (verbose) 
  {
    cout << "... mean appearance size : " << meanVector.rows << " x " << meanVector.cols << endl;
    cout << "... data size            : " << data.rows << " x " << data.cols << endl;
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

  // Compose an appearance param set from eigenvalues
  return ModelEncoder(meanAppearance.toColVector(), pca.eigenvectors);
}

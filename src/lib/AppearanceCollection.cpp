#include "AppearanceCollection.h"

AppearanceCollection::AppearanceCollection(const vector<Appearance*>& apps, bool isVerbose)
: ModelCollection(isVerbose)
{
  //this->verbose = isVerbose;
  vector<BaseModel*> vs;
  for (auto app : apps)
  {
    vs.push_back(app);
  }
  this->items = vs;
}

AppearanceCollection::AppearanceCollection(const AppearanceCollection& original)
: ModelCollection(original.verbose)
{
  //this->verbose = original.verbose;
  vector<BaseModel*> vs;
  for (auto model : original.items)
  {
    auto app = dynamic_cast<Appearance*>(model);
    BaseModel* cloned = new Appearance(*app);
    vs.push_back(cloned);
  }
  this->items = vs;
}

Mat AppearanceCollection::toMat() const
{
  // Each element has to have equal boundary
  //
  Mat frontMat = this->items[0]->toRowVector();
  int N = this->items.size();
  int M = frontMat.cols;
  auto type = frontMat.type();
  Mat m = Mat(N, M, type);
  int j = 0;
  for (auto item : this->items)
  {
    item->toRowVector().row(0).copyTo(m.row(j));
    ++j;
  }
  return m;
}

Mat AppearanceCollection::covariance(const BaseModel* mean) const
{
  Mat frontMat = this->items[0]->toRowVector();
  int M = frontMat.cols;
  int N = this->items.size();
  Mat cov = Mat::zeros(M, M, CV_32FC3);
  Mat meanVector = mean->toRowVector();
  for (auto item : this->items)
  {
    auto app = dynamic_cast<Appearance*>(item);
    auto res = app->toRowVector() - meanVector;
    cov = cov + res * res.t();
  }
  return (1/(double)N) * cov;
}

void AppearanceCollection::normaliseRotation()
{
  cout << YELLOW << "AppearanceCollection::normaliseRotation is not implemented"  << RESET << endl;
}

unique_ptr<ModelCollection> AppearanceCollection::clone() const
{
  #ifdef DEBUG
  cout << "[DEBUG] ... AppearanceCollection::clone @" << getUID() << endl;
  #endif
  vector<BaseModel*> vs;
  for (auto model : this->items)
  {
    auto app = dynamic_cast<Appearance*>(model);
    BaseModel* cloned = new Appearance(*app);
    vs.push_back(cloned);
  }
  unique_ptr<ModelCollection> newSet(new AppearanceCollection(vs, this->verbose));
  return newSet;
}
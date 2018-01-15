#include "AppearanceCollection.h"

AppearanceCollection::AppearanceCollection(const vector<Appearance*>& apps)
: ModelCollection()
{
  vector<BaseModel*> vs;
  for (auto app : apps)
  {
    this->items.push_back(new Appearance(*app));
  }
}

AppearanceCollection::AppearanceCollection(const AppearanceCollection& original)
: ModelCollection()
{
  for (auto model : original.items)
  {
    auto app = dynamic_cast<Appearance*>(model);
    BaseModel* cloned = new Appearance(*app);
    this->items.push_back(cloned);
  }
}

Mat AppearanceCollection::toMat() const
{
  // Each element has to have equal boundary

  Mat frontMat = this->items[0]->toRowVector();
  int N = this->items.size();
  int M = frontMat.cols;
  auto type = frontMat.type();

  // TAODEBUG:
  cout << "row mat : " << frontMat.rows << " x " << frontMat.cols << endl;

  Mat m = Mat(N, M, type);

  int j = 0;
  for (auto item : this->items)
  {
    // TAOTODO: Maybe this is flawed
    item->toRowVector().row(0).copyTo(m.row(j));
    ++j;
  }
  return m;
}

Mat AppearanceCollection::covariance(const BaseModel* mean) const
{
  const Appearance* meanAppearance = static_cast<const Appearance*>(mean);

  Mat meanVector = meanAppearance->toRowVector();
  int M = meanVector.rows;
  int N = this->items.size(); 
  Mat cov = Mat::zeros(M, M, CV_64FC1);

  int n = 0;
  for (auto item : this->items)
  {
    #ifdef DEBUG
    cout << "... cov #" << n << " of " << N << endl;
    n++;
    #endif

    auto app = static_cast<Appearance*>(item);
    auto res = app->toRowVector() - meanVector;

    cov = cov + res * res.t();
  }
  return (1/(double)N) * cov;
}

unique_ptr<ModelCollection> AppearanceCollection::resizeTo(double newScale) const
{
  vector<Appearance*> newApps;
  for (auto iter : this->items)
  {
    Appearance* app = dynamic_cast<Appearance*>(iter);
    Appearance* newApp = new Appearance(*app);
    newApp->resizeTo(newScale);
    newApps.push_back(newApp);
  }

  unique_ptr<ModelCollection> ptr(new AppearanceCollection(newApps));
  return ptr;
}

unique_ptr<ModelCollection> AppearanceCollection::toShapeCollection() const
{
  vector<Shape*> listShapes;
  for (auto iter : this->items)
  {
    Appearance* app = dynamic_cast<Appearance*>(iter);
    auto shape = app->getShape();
    listShapes.push_back(new MeshShape(shape));
  }

  unique_ptr<ModelCollection> ptr(new ShapeCollection(listShapes));
  return ptr;
}

void AppearanceCollection::normaliseRotation()
{
  #ifdef DEBUG
  cout << "AppearanceCollection::normaliseRotation"  << endl;
  #endif

  // Find the shape with neutral rotation
  auto shapes = this->toShapeCollection();
  shapes->normaliseRotation();
  vector<BaseModel*> items = shapes->getItems();

  // NOTE: The items are stored as [[Shape]], instead of expected [[MeshShape]]
  // so we have to create a new mesh shape on it manually
  auto mean = dynamic_cast<Shape*>(items.front()); // TAOTOREVIEW: Find out why [[MeshShape]] doesn't work properly
  MeshShape neutralShape(*mean);

  // Then align the texture part onto the neutral shape
  #ifdef DEBUG
  cout << "Re-aligning appearance texture onto normalised shape" << endl;
  #endif

  vector<Appearance*> alignedAppearances;
  int N = shapes->size();
  for (int n=0; n<N; n++)
  {
    cout << n << " of " << N << endl; // TAODEBUG:
    Appearance* original = static_cast<Appearance*>(this->items[n]);
    original->realignTo(neutralShape);    
  }
}

double AppearanceCollection::sumProcrustesDistance(const BaseModel* targetModel) const
{
  #ifdef DEBUG
  cout << "AppearanceCollection::sumProcrustesDistance" << endl;
  #endif

  // TAOTODO:
}

ModelEncoder AppearanceCollection::pca(const BaseModel* mean) const
{
  #ifdef DEBUG
  cout << GREEN << "[Computing Appearance::PCA]" << RESET << endl;
  #endif

  // Compute smaller version of zigma matrix which conforms:
  //
  // Zigma = (1/s) * (G_T . G)
  //
  //        *instead of zigma = (1/s) * (G . G_T) which is dimentionally large
  //
  // where G = [(g1 - mean) ... (gN - mean)]

  Mat meanVector = mean->toRowVector();
  Mat data       = this->toMat();

  #ifdef DEBUG
  cout << "... mean model size : " << meanVector.size() << endl;
  cout << "... data size       : " << data.size() << endl;
  #endif

  // TAOTODO: alter this given the equation : zigma = ...
  auto pca = PCA(data, meanVector, CV_PCA_DATA_AS_ROW);

  // Collect lambdas
  // TAOTOREVIEW: Take only highest K lambda where K<N
  
  #ifdef DEBUG
  cout << "... eigenvalues  : " << pca.eigenvalues.size() << endl;
  cout << "... eigenvectors : " << pca.eigenvectors.size() << endl;
  #endif

  // Compose a shape param set from eigenvalues
  return ModelEncoder(mean->toColVector(), pca.eigenvectors);
}

unique_ptr<ModelCollection> AppearanceCollection::clone() const
{
  #ifdef DEBUG
  cout << "BaseModelAppearanceCollection::clone @" << getUID() << endl;
  #endif
  vector<BaseModel*> vs;
  for (auto model : this->items)
  {
    auto app = dynamic_cast<Appearance*>(model);
    BaseModel* cloned = new Appearance(*app);
    vs.push_back(cloned);
  }
  unique_ptr<ModelCollection> newSet(new AppearanceCollection(vs));
  return newSet;
}
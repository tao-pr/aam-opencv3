#include "Appearance.h"

Appearance::Appearance(const Appearance& another)
{
  this->graphic = Mat(another.graphic.size(), another.graphic.type());
  this->mesh = another.mesh;
  another.graphic.copyTo(this->graphic);
  reinitTextures();
}

Appearance::Appearance(const MeshShape& shape, const Mat& img)
{
  auto bound = shape.getBound();
  this->graphic = Mat(bound.size(), img.type());
  this->mesh = MeshShape(shape);
  img.copyTo(this->graphic);
  reinitTextures();
}

void Appearance::reinitTextures()
{
  this->textureList.clear();  
  auto triangles = this->mesh.getTriangles();

  for (auto triangle : triangles)
  {
    this->textureList.push_back(Texture(triangle, &mesh.mat, &this->graphic));
  }
}

const double Appearance::procrustesDistance(const BaseModel* that) const
{
  #ifdef DEBUG
  cout << "Appearance::procrustesDistance" << endl;
  #endif
  const auto thatApp = dynamic_cast<const Appearance*>(that);
  Mat thisMat  = this->toRowVector();
  Mat thatMat  = thatApp->toRowVector();

  return Aux::sqrt(Aux::square(sum(thisMat - thatMat)[0]));
}

Mat Appearance::render(IO::GenericIO* io, Mat background, bool withVertices, bool withEdges, double scaleFactor, Point2d recentre) const
{
  Size size = background.size();
  
  Mat canvas = Mat(size.height, size.width, CV_64FC3);
  background.copyTo(canvas);

  #ifdef DEBUG
  if (this->textureList.size()==0)
    cout << YELLOW << "Texture list is empty, unable to render anything." << RESET << endl;
  #endif

  for (auto texture : this->textureList)
  {
    canvas = texture.render(io, canvas, withVertices, withEdges);
  }

  return canvas;
}

unique_ptr<BaseModel> Appearance::clone() const
{
  unique_ptr<BaseModel> anotherCopy(new Appearance(*this));
  return anotherCopy;
}

Mat Appearance::toRowVector() const
{
  /**
   * NOTE: assume the matrix contains 3 channels
   */
  auto bound = this->mesh.getBound();
  auto N = bound.width * bound.height;

  Mat channels[3];
  split(this->graphic(bound), channels);
  Mat row(1, N*3, CV_8UC1);

  // Concatenate all row vectors
  Mat channelRows[3];
  for (int i=0; i<3; i++)
  {
    channelRows[i] = channels[i].reshape(1,1);
    channelRows[i].copyTo(row(Rect(N*i, 0, N, 1)));
  }

  Mat rowDouble = Mat(row.size(), CV_64FC1);
  row.convertTo(rowDouble, CV_64FC1);

  return rowDouble;
}

Mat Appearance::toRowVectorReduced(int maxSize) const 
{
  assert(maxSize % 3 == 0);
  Mat rowVec = toRowVector();
  int N = rowVec.cols/3;
  int K = maxSize/3;

  // Chop original vector into 3 different channels,
  // Then shrink each of them before concatenating the results.
  Mat reduced(1, maxSize, CV_64FC1);
  Mat rowComponents[3];
  Mat reducedComponents[3];
  for (int i=0; i<3; i++)
  {
    rowComponents[i] = rowVec(Rect(i*N, 0, N, 1));
    reducedComponents[i] = Mat(1, K, CV_64FC1);
    resize(rowComponents[i], reducedComponents[i], reducedComponents[i].size());
    reducedComponents[i].copyTo(reduced(Rect(i*K, 0, K, 1)));
  }

  return reduced;
}

Mat Appearance::toColVector() const 
{
  auto rowVec = this->toRowVector();
  return rowVec.t();
}

void Appearance::realignTo(MeshShape& newShape)
{
  assert(this->mesh.mat.rows == newShape.mat.rows);

  auto originalVertices = this->mesh.toPoints();
  auto targetVertices = newShape.toPoints();
  auto targetTriangles = newShape.getTriangles();

  assert(this->textureList.size() == targetTriangles.size());

  // Warp the attached graphic onto the target
  const int span = 16;
  auto newSize = newShape.getSpannedSize();
  Mat warped = Mat::zeros(newSize.height + span, newSize.width + span, CV_8UC3);
  for (int ti=0; ti<targetTriangles.size(); ti++)
  {
    this->textureList[ti].realignTo(targetTriangles[ti], &newShape.mat, &warped);
  }

  // Move all vertices to match their corresponding counterparts in new shape
  for (int n=0; n<originalVertices.size(); n++)
  {
    auto displacement = targetVertices[n] - originalVertices[n];
    this->mesh.moveVertex(n, displacement);
  }

  // TAOTOREVIEW: Crop the excessive whitespace (spanned) from [warped]
  
  // Replace the graphic with the warped one
  this->graphic = warped;
}

void Appearance::recentre(Point2d t)
{
  auto bound = this->mesh.getBound();
  this->mesh = MeshShape(this->mesh >> t);

  // Correction of boundary
  auto newBound = this->mesh.getBound();
  newBound.width = bound.width;
  newBound.height = bound.height; 
  int h = this->graphic.rows + t.y;
  int w = this->graphic.cols + t.x;
  Mat newGraphic = Mat::zeros(h, w, this->graphic.type());

  #ifdef DEBUG
  cout << "Appearance recentering : " << bound << " => " << newBound << endl;
  #endif

  this->graphic(bound).copyTo(newGraphic(newBound));
  swap(this->graphic, newGraphic);
}

void Appearance::resizeTo(double newScale)
{
  auto centre = this->mesh.centroid();
  double originalScale = this->mesh.getScale();
  double ratio = newScale / originalScale;

  #ifdef DEBUG
  cout << "Appearance::resizeTo : from " << originalScale << " -> " << newScale << " (scale = " << ratio << ")" << endl;
  #endif

  // Resize shape without translation
  this->mesh = MeshShape(this->mesh * ratio);

  // Resize texture without translation
  auto bound = this->mesh.getBound();
  Size newSize(bound.x + bound.width, bound.y + bound.height);
  resize(this->graphic, this->graphic, newSize, 0, 0, INTER_NEAREST);
}

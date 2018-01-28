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
  #ifdef DEBUG
  cout << "Appearance::reinitTextures" << endl;
  #endif

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
  cout << "BaseModelAppearance::procrustesDistance" << endl;
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
  Mat row = this->graphic(bound).clone().reshape(1,1);
  Mat rowDouble = Mat(row.size(), CV_64FC1);
  row.convertTo(rowDouble, CV_64FC1);

  return rowDouble;
}

Mat Appearance::toRowVectorReduced(int maxSize) const 
{
  Mat rowVec = toRowVector();
  Mat reduced(1, maxSize, CV_64FC1);
  resize(rowVec, reduced, Size(1, maxSize));
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
  auto newSize = newShape.getSpannedSize();
  Mat warped = Mat::zeros(newSize, CV_8UC3);
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
  
  // Replace the graphic with the warped one
  this->graphic = warped;
}

void Appearance::resizeTo(double newScale)
{
  auto centre = this->mesh.centroid();

  // Find the current scale
  auto points = this->mesh.toPoints();
  double maxDist = 0;
  for (auto p : points)
  {
    double dist = Aux::sqrt(Aux::squareDistance(p, centre));
    maxDist = max(dist, maxDist);
  }

  double ratio = newScale / Aux::sqrt(maxDist);

  #ifdef DEBUG
  cout << "Appearance::resizeTo : from " << maxDist << " -> " << newScale << " (scale = " << ratio << ")" << endl;
  #endif


  // Resize shape without translation
  this->mesh = MeshShape(this->mesh * ratio);


  // Resize texture without translation
  auto bound = this->mesh.getBound();
  Size newSize(bound.x + bound.width, bound.y + bound.height);
  resize(this->graphic, this->graphic, newSize, 0, 0, INTER_NEAREST);
}

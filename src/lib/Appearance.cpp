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
  this->graphic = Mat(img.size(), img.type());
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
  cout << "[DEBUG] ... Appearance::procrustesDistance" << endl;
  #endif
  const auto thatApp = dynamic_cast<const Appearance*>(that);
  Mat thisMat  = this->toRowVector();
  Mat thatMat  = thatApp->toRowVector();
  Mat A = Mat(thisMat.size(), CV_8UC1);
  Mat B = Mat(thatMat.size(), CV_8UC1);
  cvtColor(thisMat, A, COLOR_BGR2GRAY);
  cvtColor(thatMat, B, COLOR_BGR2GRAY);
  return Aux::sqrt(Aux::square(sum(A - B)[0]));
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
  auto roi = this->mesh.getBound();
  Mat m = this->graphic(roi).clone();
  return m.reshape(this->graphic.channels(), m.rows * m.cols);
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
  auto b = newShape.getBound();
  Size newSize(b.x + b.width, b.y + b.height);
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


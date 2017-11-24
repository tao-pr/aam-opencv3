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
  this->textureList.clear();  
  auto triangles = this->mesh.getTriangles();

  for (auto triangle : triangles)
  {
    this->textureList.push_back(Texture(triangle, &this->graphic));
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

  // TAOTODO: Delete following
  // vector<Vec3b> series;
  // // Crop the texture inside the triangular boundary
  // // concatnate them into a 1-D vector
  // auto bound = this->mesh.getBound();
  // const int xA = bound.x;
  // const int xB = bound.x + bound.width;
  // const int yA = bound.y;
  // const int yB = bound.y + bound.height;

  // for (int j=yA; j<yB; j++)
  //   for (int i=xA; i<xB; i++)
  //   {
  //     // In case the pixel is absent from the attached texture,
  //     // impute with zero
  //     if (i<imgRef->cols && j<imgRef->rows)
  //       series.push_back(imgRef->at<Vec3b>(j,i));
  //     else 
  //       series.push_back(Vec3b(0,0,0));
  //   }

  // // TAOTODO: Any better way to create Mat from continuous memory block directly?
  // const int N = series.size();
  // Mat out = Mat(1, N, this->imgRef->type());
  // for (int n=0; n<N; n++)
  //   out.at<Vec3b>(0,n) = series[n];
  // return out;
}

Mat Appearance::toColVector() const 
{
  auto rowVec = this->toRowVector();
  return rowVec.t();
}

void Appearance::realignTo(const MeshShape& newShape)
{
  // TAOTODO:
}


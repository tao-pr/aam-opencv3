#include "AppearanceModel.h"

Appearance::Appearance(const MeshShape& shape, Mat* img)
{
  this->imgRef = img;
  this->mesh = MeshShape(shape);
  reinitiateTextures();
}

void Appearance::reinitiateTextures()
{
  this->textureList.clear();  
  auto triangles = this->mesh.getTriangles();

  for (auto triangle : triangles)
  {
    this->textureList.push_back(Texture(triangle, this->imgRef));
  }
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

Mat Appearance::toRowVector() const
{
  vector<Vec3b> series;
  // Crop the texture inside the triangular boundary
  // concatnate them into a 1-D vector
  auto bound = this->mesh.getBound();
  const int xA = bound.x;
  const int xB = bound.x + bound.width;
  const int yA = bound.y;
  const int yB = bound.y + bound.height;

  for (int j=yA; j<yB; j++)
    for (int i=xA; i<xB; i++)
    {
      // In case the pixel is absent from the attached texture,
      // impute with zero
      if (i<imgRef->cols && j<imgRef->rows)
        series.push_back(imgRef->at<Vec3b>(j,i));
      else 
        series.push_back(Vec3b(0,0,0));
    }

  // TAOTODO: Any better way to create Mat from continuous memory block directly?
  const int N = series.size();
  Mat out = Mat(1, N, this->imgRef->type());
  for (int n=0; n<N; n++)
    out.at<Vec3b>(0,n) = series[n];
  return out;
}


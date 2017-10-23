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
  cout << "Querying triangles ..." << endl; // TAODEBUG:
  auto triangles = this->mesh.getTriangles();

  // TAOTODO: FIX THIS
  cout << "num triangles for textures = " << triangles.size() << endl; // TAODEBUG:

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
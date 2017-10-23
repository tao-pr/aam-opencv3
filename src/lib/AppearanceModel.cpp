#include "AppearanceModel.h"

void Appearance::reinitiateTextures()
{
  this->textureList.clear();
  
  // TAODEBUG:
  cout << "Reading triangles from mesh" << endl;

  auto triangles = this->mesh.getTriangles();

  // TAODEBUG:
  cout << "triangles : " << triangles.size() << endl;

  for (auto triangle : triangles)
  {
    this->textureList.push_back(Texture(triangle, this->imgRef));
  }
}

Mat Appearance::render(IO::GenericIO* io, Mat background, bool withVertices, bool withEdges, double scaleFactor, Point2d recentre) const
{
  Size size = background.size();
  Mat canvas = Mat(size.height, size.width, CV_64FC3);
  cout << "cloning background ..." << endl; // TAODEBUG:
  background.copyTo(canvas);

  cout << "rendering textures ..." << endl; // TAODEBUG:
  for (auto texture : this->textureList)
  {
    cout << "rendering texture " << endl; // TAODEBUG:
    texture.render(io, canvas, withVertices, withEdges);
  }

  return canvas;
}
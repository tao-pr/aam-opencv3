#include "AppearanceModel.h"

void Appearance::reinitiateTextures()
{
  this->textureList.clear();
  auto triangles = this->mesh.getTriangles();
  for (auto triangle : triangles)
  {
    this->textureList.push_back(Texture(triangle, this->imgRef));
  }
}

Mat Appearance::render(IO::GenericIO* io, Mat background, double scaleFactor, Point2d recentre) const
{

}
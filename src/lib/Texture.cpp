#include "Texture.h"

void Texture::save(const string path) const
{
  // TAOTODO:
}

void Texture::load(const string path)
{

}

Mat Texture::render(IO::GenericIO* io, Mat background, double scaleFactor, Point2d recentre) const
{
  // TAOTODO:
}

Texture Texture::operator*(double scale) const
{
  return Texture(this->shape, this->img * scale);
}
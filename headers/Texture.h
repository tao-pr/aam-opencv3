/**
 * Generic Texture Model
 */

#ifndef TEXTURE_MODEL
#define TEXTURE_MODEL

#include "master.h"
#include "IO.h"
#include "aux.h"

class Texture
{
private:
protected:
public:
  Mat img;

  inline Texture(){};
  inline Texture(const Mat& a){ a.copyTo(this->img); };
  inline Texture(const Texture& original){ original.img.copyTo(this->img); };
  virtual inline ~Texture(){};

  // --------- General properties -------


  // --------- I/O ------------
  virtual void save(const string path) const;
  virtual void load(const string path);
  virtual Mat render(IO::GenericIO* io, Mat background, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;

  //------ Operators -------
  Texture operator*(double scale) const;
};
/**
 * Generic Texture Model
 */

#ifndef TEXTURE_MODEL
#define TEXTURE_MODEL

#include "master.h"
#include "IO.h"
#include "aux.h"
#include "Triangle.h"

/**
 * Texture coupled with a triangular face
 */
class Texture
{
private:
protected:
public:
  Mat *img;
  Triangle bound;

  inline Texture(const Triangle& bound, Mat* a) : img(a), bound(bound) {};
  inline Texture(const Texture& original) : img(original.img), bound(original.bound) {};
  virtual inline ~Texture(){};

  // --------- General properties -------


  // --------- I/O ------------
  virtual void save(const string path) const;
  virtual void load(const string path);
  virtual Mat render(IO::GenericIO* io, Mat background, bool withVertices=true, bool withEdges=true, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;

  //------ Operators / Transformations -------
  Texture realignTo(const Triangle &newBound, Mat* m) const;
};

#endif
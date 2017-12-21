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
  Mat *img; // The memory block dedicatedly belongs to the [Texture] instance
  Mat *vertexRef; // The memory block refers to the shared vertex mat
  Triangle bound;

  // TAOTODO: Should have a weak binding to vertex mat 
  inline Texture(const Triangle& bound, Mat* v, Mat* a) : img(a), vertexRef(v), bound(bound) {};
  inline Texture(const Texture& original) : img(original.img), vertexRef(original.vertexRef), bound(original.bound) {};
  virtual inline ~Texture(){};

  // --------- General properties -------
  void reassignVertexRef( Mat* v) { this->vertexRef = v; };

  // --------- I/O ------------
  virtual void save(const string path) const;
  virtual void load(const string path);
  virtual Mat render(IO::GenericIO* io, Mat background, bool withVertices=true, bool withEdges=true, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;

  //------ Operators / Transformations -------
  Texture realignTo(const Triangle &newBound, Mat* newVertexRef, Mat* m) const;
};

#endif
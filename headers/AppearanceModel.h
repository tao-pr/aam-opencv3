#ifndef APPEARANCE_MODEL
#define APPEARANCE_MODEL

#include "master.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Texture.h"

/**
 * To this API, Appearance is a mixture of Shape and associated 
 * Texture inside each triangulated faces.
 */
class Appearance
{
private:
protected:
  Mat* imgRef; // Reference to an appearance image (external variable)
  MeshShape mesh;
  vector<Texture> textureList;

  void reinitiateTextures();
public:
  Appearance(const MeshShape& shape, Mat* img) : imgRef(img), mesh(shape) { reinitiateTextures(); };
  virtual inline ~Appearance(){};

  //------ I/O --------------
  Mat render(IO::GenericIO* io, Mat background, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;
};

#endif
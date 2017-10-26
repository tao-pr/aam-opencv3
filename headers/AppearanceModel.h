#ifndef APPEARANCE_MODEL
#define APPEARANCE_MODEL

#include "master.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Texture.h"

/**
 * A conjunction between [Texture] and its assoicated [Shape]
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
  Appearance(const MeshShape& shape, Mat* img);
  virtual inline ~Appearance(){};

  //------ I/O --------------
  Mat render(IO::GenericIO* io, Mat background, bool withVertices=true, bool withEdges=true, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;
  Mat toRowVector() const;
  // TAOTODO: Add a method toPiecewiseVector which returns a vector of each triangle
};

#endif
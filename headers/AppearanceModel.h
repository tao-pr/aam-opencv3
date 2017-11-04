#ifndef APPEARANCE_MODEL
#define APPEARANCE_MODEL

#include "master.h"
#include "GenericModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Texture.h"

/**
 * Statistical model of bounded texture
 */
class Appearance : public GenericModel
{
private:
protected:
  Mat* imgRef; // Reference to an appearance image (external variable)
  MeshShape mesh;
  vector<Texture> textureList;

  void reinitTextures();
public:
  Appearance(const MeshShape& shape, Mat* img);
  virtual inline ~Appearance(){};

  //------ I/O --------------
  Mat render(IO::GenericIO* io, Mat background, bool withVertices=true, bool withEdges=true, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;
  Mat toRowVector() const;
  Mat toColVector() const;

  // TAOTOREVIEW:
  virtual inline void save(const string path) const {};
  virtual inline void load(const string path) {};
};

#endif
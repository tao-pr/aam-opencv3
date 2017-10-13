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
  MeshShape mesh;
public:
  Appearance(){};
  virtual inline ~Appearance(){};
};

#endif
/**
 * PCA of MeshShape
 */

#ifndef PCA_SHAPE
#define PCA_SHAPE

#include "IO.h"
#include "aux.h"
#include "MeshShape.h"

class PCAShape : public MeshShape 
{
protected:
  unsigned int maxComponents;
  PCA pca;
public:
  PCAShape(){};
  PCAShape(unsigned int maxNumComponents) : maxComponents(maxNumComponents) {};
  PCAShape(const PCAShape& original);
  virtual ~PCAShape();
};

#endif
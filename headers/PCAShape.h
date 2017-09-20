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
  Mat pcaShape;
public:
  PCAShape(){};
  PCAShape(unsigned int maxNumComponents) : maxComponents(maxNumComponents) {};
  PCAShape(const PCAShape& original);
  PCAShape(vector<Point2d>& vs);
  virtual ~PCAShape();
};

#endif
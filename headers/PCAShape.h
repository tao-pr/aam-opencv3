/**
 * PCA of MeshShape
 */

#ifndef PCA_SHAPE
#define PCA_SHAPE

#include "IO.h"
#include "aux.h"
#include "Shape.h"

class PCAShape : public MeshShape 
{
private:
protected:
  PCA pca;
public:
  PCAShape();
  PCAShape(const PCAShape& original);
  ~PCAShape();
};

#endif
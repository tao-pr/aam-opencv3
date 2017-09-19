#include "PCAShape.h"

PCAShape::PCAShape(const PCAShape& original) : MeshShape(original)
{
  this->maxComponents = original.maxComponents;
}

PCAShape::PCAShape(vector<Point2d>& vs, int maxNumComponents) : MeshShape(vs)
{
  this->maxComponents = maxNumComponents;
  // TAOTODO: Compute PCA of [vs]
}
#include "MeshShapeCollection.h"

MeshShape MeshShapeCollection::mean() const
{
  // Apply Generalised Procrustes analysis on the list of shapes
  // to find the mean shape without translation, planar rotation
  Mat meanShape = this->items[0].toMat();

  // Iterate until converges
  // TAOTODO:
}
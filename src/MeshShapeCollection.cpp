#include "MeshShapeCollection.h"

MeshShapeCollection::MeshShapeCollection(const vector<MeshShape>& shapes)
{
  this->items = shapes;
}

MeshShape MeshShapeCollection::mean() const
{
  // Apply Generalised Procrustes analysis on the list of shapes
  // to find the mean shape without translation, planar rotation
  MeshShape meanShape = this->items[0];

  // Iterate until converges
  for (auto shapeIter=this->items.begin()+1; shapeIter != this->items.end(); shapeIter++)
  {
    auto unit = ShapeIter->normalise();

    // TAOTODO:
  }

  return meanShape;
}
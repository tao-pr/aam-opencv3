#include "ShapeCollection.h"

MeshShape ShapeCollection::procrustes() const
{
  // Rescale each shape so the centroid size = 1
  // and translate to the centroid
  vector<Shape> scaled;
  for (auto shape : this->items)
  {
    auto centroid = shape.centroid();
    auto cdist    = shape.sumSquareDistanceToPoint(centroid);
    scaled.push_back((shape * (1.0/cdist)) >> centroid);
  }

  // TAOTODO:


  return meanShape;
}
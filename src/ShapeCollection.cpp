#include "ShapeCollection.h"

ShapeCollection::ShapeCollection(const vector<Shape>& shapes)
{
  this->items = shapes;
}

ShapeCollection::ShapeCollection(const ShapeCollection& original)
{
  this->items = original.items;
}

MeshShape ShapeCollection::mean() const
{
  // Take the first shape from the collection as initial mean
  Shape sumShape = this->items[0];

  // Sum all shapes and compute the means, skip the first one
  for (auto shapeIter=this->items.begin()+1; shapeIter != this->items.end(); shapeIter++)
  {
    sumShape = sumShape >> *shapeIter;
  }
  auto meanShape = sumShape * (1/(double)this->items.size());

  // Compute residual shapes
  vector<Shape> residualShapes;
  for (auto shape : this->items)
  {
    residualShapes.push_back(shape - meanShape);
  }

  return meanShape;
}
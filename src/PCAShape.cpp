#include "PCAShape.h"

PCAShape::PCAShape(const PCAShape& original) : MeshShape(original)
{
  this->maxComponents = original.maxComponents;
}
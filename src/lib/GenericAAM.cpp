#include "GenericAAM.h"

GenericTrainset::GenericTrainset(const ShapeCollection &annotations, const AppearanceCollection &appearances)
{
  this->shapeEntries = ShapeCollection(annotations);
  this->appEntries   = AppearanceCollection(appearances);
}

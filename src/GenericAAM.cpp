#include "GenericAAM.h"

GenericTrainset::GenericTrainset(const ShapeCollection &annotations, const AppearanceCollection &appearances)
{
  this->shapeEntries = annotations;
  this->appEntries   = appearances;
}

#include "AAM.h"

GenericTrainset::GenericTrainset(const ShapeCollection &annotations, const vector<Mat> &imgs)
{
  this->shapeEntries = annotations;
  // TAOTODO: Store MeshAppearanceCollection
}

GenericAAM GenericTrainset::train(bool verbose) const
{

}
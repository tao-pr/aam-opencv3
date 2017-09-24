#include "AAM.h"

GenericTrainset::GenericTrainset(const vector<MeshShape> &annotations, const vector<Mat> &imgs)
{
  this->shapeEntries = new MeshShapeCollection(annotations);
  // TAOTODO: Store MeshAppearanceCollection
}

GenericTrainset::~GenericTrainset()
{
  delete this->shapeEntries;
}

GenericAAM GenericTrainset::train() const
{

}
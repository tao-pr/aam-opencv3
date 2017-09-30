#include "Test.h"

int main(int argc, char** argv)
{
  // TAOTODO: Following should be parsed from args
  const int TRAIN_SET_SIZE = 8;
  const int SHAPE_SIZE = 3;

  const ShapeCollection trainset = initialShapeCollection(TRAIN_SET_SIZE, SHAPE_SIZE);

  // Render each shape onto the same plane
  auto io = IO::WindowIO("shapes");
  trainset.renderShapeVariation(&io);
  waitKey(0);
}
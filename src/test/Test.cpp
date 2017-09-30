#include "Test.h"

int main(int argc, char** argv)
{
  // TAOTODO: Following should be parsed from args
  const int TRAIN_SET_SIZE = 8;
  const int SHAPE_SIZE = 3;

  const ShapeCollection trainset = initialShapeCollection(TRAIN_SET_SIZE, SHAPE_SIZE);

  // Render each shape onto the same plane
  auto io = IO::WindowIO("shapes");
  trainset.renderShapeVariation(&io, Size(CANVAS_SIZE, CANVAS_SIZE));

  // Generate procrustes analysis on the shape set
  auto ioNrm = IO::WindowIO("norm-shapes");
  auto normalisedTrainset = trainset.normaliseScalingTranslation();
  normalisedTrainset
    //.translateBy(Point2d(-CANVAS_HALFSIZE, -CANVAS_HALFSIZE))
    .renderShapeVariation(
      &ioNrm, 
      Size(CANVAS_SIZE, CANVAS_SIZE));

  // TAODEBUG:
  for (auto s : normalisedTrainset.getItems())
  {
    cout << YELLOW << "***********************" << RESET << endl;
    cout << s.mat << endl;
  }
  
  waitKey(0);
}
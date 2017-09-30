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

  // Remove translations & scalings
  auto ioNrm = IO::WindowIO("norm-shapes (re-scaled)");
  auto normalisedTrainset = trainset.normaliseScalingTranslation();
  normalisedTrainset.renderShapeVariation(
    &ioNrm, 
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));

  cout << GREEN << "[Shapes without translation and scaling]" << RESET << endl;
  for (auto s : normalisedTrainset.getItems())
  {
    cout << YELLOW << "***********************" << RESET << endl;
    cout << s.mat << endl;
  }

  // Remove rotations
  auto ioPc = IO::WindowIO("procrustes shapes (re-scaled)");
  auto procrustesSet = normalisedTrainset.normaliseRotation();

  cout << GREEN << "[Shapes without rotation]" << RESET << endl;
  for (auto s : procrustesSet.getItems())
  {
    cout << YELLOW << "***********************" << RESET << endl;
    cout << s.mat << endl;
  }
  
  waitKey(0);
}
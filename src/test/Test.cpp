#include "Test.h"

int main(int argc, char** argv)
{
  // TAOTODO: Following should be parsed from args
  const int TRAIN_SET_SIZE = 16;
  const int SHAPE_SIZE     = 5;
  const double TOL         = 1e-3;
  const int MAX_ALIGN_ITER = 10;

  const ShapeCollection trainset = initialShapeCollection(TRAIN_SET_SIZE, SHAPE_SIZE);

  // Render each shape onto the same plane
  auto io = IO::WindowIO("shapes");
  trainset.renderShapeVariation(&io, Size(CANVAS_SIZE, CANVAS_SIZE));

  // Remove translations & scalings
  auto ioNrm = IO::WindowIO("scaling + translated");
  auto scaledSet = trainset.normaliseScalingTranslation();
  scaledSet.renderShapeVariation(
    &ioNrm, 
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));

  // cout << GREEN << "[Shapes without translation and scaling]" << RESET << endl;
  // for (auto s : scaledSet.getItems())
  // {
  //   cout << YELLOW << "***********************" << RESET << endl;
  //   cout << s.mat << endl;
  // }

  // Remove rotations
  auto ioPc = IO::WindowIO("rotated");
  auto rotatedSet = scaledSet.normaliseRotation();
  rotatedSet.renderShapeVariation(
    &ioPc,
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));

  // cout << GREEN << "[Shapes without rotation]" << RESET << endl;
  // for (auto s : rotatedSet.getItems())
  // {
  //   cout << YELLOW << "***********************" << RESET << endl;
  //   cout << s.mat << endl;
  // }

  // Find the mean shape by Procrustes Analysis
  // and align all shapes to that mean
  auto meanAndAligned = rotatedSet.procrustesMeanShape(TOL, MAX_ALIGN_ITER);
  auto meanShape      = get<0>(meanAndAligned);
  auto alignedSet     = get<1>(meanAndAligned);
  auto ioAl   = IO::WindowIO("Aligned");
  auto ioMean = IO::WindowIO("mean");
  
  // Re-scale and re-centre the mean shape before rendering
  //((meanShape * Aux::square(CANVAS_HALFSIZE)) >> Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE))
  meanShape.recentreAndScale(Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE), Aux::square(CANVAS_HALFSIZE))
    .render(&ioMean, Mat(CANVAS_SIZE, CANVAS_SIZE, CV_8UC3, Scalar(100,40,5)));
  
  alignedSet.renderShapeVariation(
    &ioAl,
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));
  waitKey(0);
}
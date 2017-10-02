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
  auto ioNrm = IO::WindowIO("scaling + translated", Point(CANVAS_SIZE+10, 0));
  auto scaledSet = trainset.normaliseScalingTranslation();
  scaledSet.renderShapeVariation(
    &ioNrm, 
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));
  moveWindow("scaling + translated", CANVAS_SIZE+10, 0);

  // cout << GREEN << "[Shapes without translation and scaling]" << RESET << endl;
  // for (auto s : scaledSet.getItems())
  // {
  //   cout << YELLOW << "***********************" << RESET << endl;
  //   cout << s.mat << endl;
  // }

  // Remove rotations
  auto ioPc = IO::WindowIO("rotated", Point(CANVAS_SIZE+20, 0));
  auto rotatedSet = scaledSet.normaliseRotation();
  rotatedSet.renderShapeVariation(
    &ioPc,
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));
  moveWindow("rotated", (CANVAS_SIZE+10)*2, 0);

  // cout << GREEN << "[Shapes without rotation]" << RESET << endl;
  // for (auto s : rotatedSet.getItems())
  // {
  //   cout << YELLOW << "***********************" << RESET << endl;
  //   cout << s.mat << endl;
  // }

  // Find the mean shape by Procrustes Analysis
  // and align all shapes to that mean
  // NOTE: We don't use the rotated set above
  auto meanAndAligned = scaledSet.procrustesMeanShape(TOL, MAX_ALIGN_ITER);
  auto meanShape      = get<0>(meanAndAligned);
  auto alignedSet     = get<1>(meanAndAligned);
  auto ioAl   = IO::WindowIO("aligned", Point(0, CANVAS_SIZE+25));
  auto ioMean = IO::WindowIO("mean", Point(CANVAS_SIZE+10, CANVAS_SIZE+25));
  
  // Re-scale and re-centre the mean shape before rendering
  meanShape
    .recentreAndScale(Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE), Aux::square(CANVAS_HALFSIZE))
    .render(&ioMean, Mat(CANVAS_SIZE, CANVAS_SIZE, CV_8UC3, Scalar(80,20,5)));
  
  alignedSet.renderShapeVariation(
    &ioAl,
    Size(CANVAS_SIZE, CANVAS_SIZE),
    Aux::square(CANVAS_HALFSIZE), // scale the distance
    Point2d(CANVAS_HALFSIZE, CANVAS_HALFSIZE));
  moveWindow("aligned", (CANVAS_SIZE+10)*3, 0);
  moveWindow("mean", 0, CANVAS_SIZE+50);

  // Calculate covariance
  auto cov = alignedSet.covariance(meanShape);
  Mat covResized;
  resize(cov, covResized, Size(CANVAS_SIZE, CANVAS_SIZE), INTER_LINEAR);
  normalize(covResized, covResized, 255, 0, NORM_L2);
  imshow("cov", covResized);
  moveWindow("cov", (CANVAS_SIZE+10),(CANVAS_SIZE+50));

  // Calculate PCA shape
  auto eigenShape = alignedSet.clone(true).pca(meanShape);

  // Encode shapes with PCA and measure the estimation errors
  int i = 0;
  cout << GREEN << "[PCA-parameterised shapes]" << RESET << endl;
  for (auto shape : alignedSet.getItems())
  {
    cout << "... Encoding shape #" << i << endl;
    auto param = eigenShape.encode(shape);
    auto encodedShape = eigenShape.decode(param);
    double error = shape.procrustesDistance(encodedShape);
    cout << "... Estimation error : " << error << endl;
    i++;
  }

  waitKey(0);
}
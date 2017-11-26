/**
 * Main test suite
 */

#include "IO.h"
#include "master.h"
#include "AAM2D.h"
#include "ShapeCollection.h"
#include "AppearanceCollection.h"
#include "MeshShape.h"
#include "Texture.h"

const bool   VERBOSE         = true;
const double CANVAS_SIZE     = 300.0;
const double CANVAS_HALFSIZE = CANVAS_SIZE / 2.0;
const double NOISE_T         = 15.0;
const double NOISE_R         = 0.665;

inline MeshShape initialMesh(int shapeSize)
{
  srand(time(NULL));
  vector<Point2d> vs;
  for (int i=0; i<shapeSize; i++)
  {
    double x = rand() % (int)CANVAS_SIZE;
    double y = rand() % (int)CANVAS_SIZE;
    vs.push_back(Point2d(x, y));
  }
  return MeshShape(vs);
}

inline unique_ptr<ShapeCollection> initialShapeCollection(int num, int shapeSize)
{
  cout << GREEN << "Generating initial shapes of size " << RESET 
    << num << " x " << shapeSize << endl;
  vector<Shape*> trainset;
  srand(time(NULL));
  for (int i=0; i<num; i++)
  {
    Mat m(shapeSize, 2, CV_64FC1);
    double nr = NOISE_R * (rand()%1000)/1000.0;
    for (int n=0; n<shapeSize; n++)
    {
      double angle = 2 * M_PI * (double)n / (double)shapeSize + nr;
      double x0 = CANVAS_HALFSIZE + (CANVAS_HALFSIZE * cos(angle));
      double y0 = CANVAS_HALFSIZE + (CANVAS_HALFSIZE * sin(angle));
      double n0 = NOISE_T * (rand()%1000)/1000.0;
      double n1 = NOISE_T * (rand()%1000)/1000.0;
      m.at<double>(n, 0) = x0 + n0 - n1;
      m.at<double>(n, 1) = y0 - n0 + n1;
    }
    trainset.push_back(new Shape(m));
  }
  unique_ptr<ShapeCollection> ptrTrainset(new ShapeCollection(trainset, VERBOSE));
  return ptrTrainset;
}

inline Mat chessPattern(int stepSize, Size size)
{
  Mat mat = Mat::zeros(size, CV_8UC3);
  for (int j=0; j<size.height; j+=stepSize)
    line(mat, Point(0, j), Point(size.width-1,j), Scalar(255,255,255), 1, CV_AA);
  for (int i=0; i<size.width; i+=stepSize)
    line(mat, Point(i, 0), Point(i, size.height-1), Scalar(255,255,255), 1, CV_AA);
  return mat;
}

inline unique_ptr<AppearanceCollection> initialAppearanceCollection(int num, int shapeSize)
{
  cout << GREEN << "Generating initial appearances of size " << RESET 
    << num << " x " << shapeSize << endl;

  // Generate a base shape and texture
  auto baseShape = MeshShape(initialMesh(shapeSize));
  auto baseTexture = chessPattern(5, Size(CANVAS_SIZE, CANVAS_SIZE));

  // Generate [n] random displacements on the base shape
  auto noiseConstraint = Point2d(15.5, 5.5);
  vector<Appearance*> appearances;
  for (int n=0; n<num; n++)
  {
    auto newShape = Shape(baseShape);
    newShape.addRandomNoise(noiseConstraint);
    auto newMesh = MeshShape(newShape);
    
    // Create an appearance on the base shape
    // then warp it onto the new shape with random noise added
    auto app = new Appearance(baseShape, baseTexture);
    app->realignTo(newMesh);
    appearances.push_back(app);
  }

  unique_ptr<AppearanceCollection> list(new AppearanceCollection(appearances));
  return list;
}
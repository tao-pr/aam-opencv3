/**
 * Main test suite
 */

#include "IO.h"
#include "master.h"
#include "AAM2D.h"
#include "ShapeCollection.h"
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

inline ShapeCollection initialShapeCollection(int num, int shapeSize)
{
  cout << GREEN << "Generating initial shapes of size " << RESET 
    << num << " x " << shapeSize << endl;
  vector<Shape> trainset;
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
    trainset.push_back(Shape(m));
  }
  return ShapeCollection(trainset, VERBOSE);
}
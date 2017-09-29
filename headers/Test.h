/**
 * Main test suite
 */

#include "IO.h"
#include "master.h"
#include "ShapeCollection.h"

const float CANVAS_SIZE = 300.0;

inline ShapeCollection initialShapeCollection(int num, int shapeSize)
{
  cout << GREEN << "Generating initial shapes of size " << RESET 
    << num << " x " << shapeSize << endl;
  vector<Shape> trainset;
  srand(time(NULL));
  for (int i=0; i<num; i++)
  {
    Mat m(shapeSize, 2, CV_64FC1);
    for (int n=0; n<shapeSize; n++)
    {
      double angle = M_PI * (double)i * 180 / (double)num;
      double x0 = shapeSize + (double)(shapeSize * cos(angle));
      double y0 = shapeSize - (double)(shapeSize * sin(angle));
      double n0 = rand()*10.0;
      double n1 = rand()*10.0;
      m.at<double>(n, 0) = x0 + n0 - n1;
      m.at<double>(n, 1) = y0 - n0 + n1;
    }
    trainset.push_back(Shape(m));
  }
  return ShapeCollection(trainset);
}
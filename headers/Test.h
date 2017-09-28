/**
 * Main test suite
 */

#include "IO.h"
#include "master.h"
#include "ShapeCollection.h"

const float CANVAS_SIZE = 300.0;

inline const ShapeCollection& initialShapeCollection(int num, int shapeSize)
{
  cout << GREEN << "Generating initial shapes of size " << RESET 
    << num << " x " << shapeSize << endl;
  vector<Shape> trainset;
  for (int i=0; i<num; i++)
  {
    Mat m(shapeSize, 2, CV_64FC1);
    for (int n=0; n<shapeSize; n++)
    {
      m.at<double>(n, 0) = (float)(rand()*CANVAS_SIZE);
      m.at<double>(n, 1) = (float)(rand()*CANVAS_SIZE);
    }
    trainset.push_back(Shape(m));
  }
  return ShapeCollection(trainset);
}
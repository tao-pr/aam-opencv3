#include "PCAShape.h"

PCAShape::PCAShape(const PCAShape& original) : MeshShape(original)
{
  this->maxComponents = original.maxComponents;
}

PCAShape::PCAShape(vector<Point2d>& vs, int maxNumComponents) : MeshShape(vs)
{
  this->maxComponents = maxNumComponents;
  Mat rawPoints = Mat(ps.size(), 2, CV_64FC1);
  for (int i=0; i<vs.size(); i++)
  {
    rawPoints.at(i,0) = vs[i].x;
    rawPoints.at(i,1) = vs[i].y;
  }
  this->pca = PCA(rawPoints, Mat(), PCA::DATA_AS_COLUMN, nComponents);
  this->pcaShape = Mat(nComponents, 2, CV_64FC1);
}
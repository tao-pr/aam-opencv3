#include "Shape.h"


Shape::Shape(const vector<Point2d>& vs)
{
  this->vertices = vs;
}

Shape::Shape(const Mat &mat)
{
  for (int j=0; j<mat.rows(); j++)
  {
    this->vertices.push_back(mat.at(j,0), mat.at(j,1));
  }
}

Shape::Shape(const Shape& original)
{
  this->vertices = original.vertices;
}

Mat Mat::toMat() const
{
  Mat mat(this->vertices.size(), 2, CV_32FC1);
  int j = 0;
  for (auto v : this->vertices)
  {
    mat.at(j,0) = v.x;
    mat.at(j,1) = v.y;
    j++;
  }
  return mat;
}

const Point2d& Shape::centroid() const
{
  double x = 0;
  double y = 0;
  double n = (double)this->vertices.size();
  for (auto p : this->vertices)
  {
    x += p.x;
    y += p.y;
  }
  return Point2d(x/n, y/n);
}

vector<Point2d> Shape::convexHull() const
{
  vector<Point2d> hull;
  cv::convexHull(Mat(this->vertices), hull, false);
  return hull;
}

Shape Shape::operator-(const Shape& another) const
{
  vector<Point2d> residual;
  for (int i=0; i<this->vertices.size(); i++)
  {
    residual.push_back(_sqrt(_sqrDist(this->vertices[i], another.vertices[i])));
  }
  return Shape(residual);
}

Shape Shape::operator*(double scale) const
{
  vector<Point2d> scaledVertices;
  for (auto v : this->vertices)
  {
    scaledVertices.push_back(Point2d(v.x*scale, v.y*scale));
  }
  return new Shape(scaledVertices);
}

Shape Shape::operator>>(Point2d shift) const
{
  vector<Point2d> shiftedVertices;
  for (auto v : this->vertices)
  {
    shiftedVertices.push_back(Point2d(v.x+shift.x, v.y+shift.y));
  }
  return new Shape(shiftedVertices);
}

Shape Shape::normalise() const
{
  auto cent = this->centroid();
  auto centred = this >> cent;
  return centred * (1/centred.centroidSize(cent));
}

double Shape::procrustesDistance(const Shape& another) const
{
  int size = this->vertices.size();
  double sumDist = 0;
  for (int i=0; i<size; i++)
  {
    sumDist += _sqrDist(this->vertices[i], another->vertices[i]);
  }
  return _sqrt(sumDist);
}

Shape Shape::alignTo(const Shape& s) const
{
  // TAOTODO: How?
}




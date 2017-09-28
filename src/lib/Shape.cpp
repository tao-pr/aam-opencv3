#include "Shape.h"

Shape::Shape(const vector<Point2d>& vs)
{
  this->mat = Mat(vs.size(), 2, CV_64FC1);
  for (int j=0; j<vs.size(); j++)
  {
    this->mat.at<double>(j,0) = vs[0].x;
    this->mat.at<double>(j,1) = vs[0].y;
  }
}

Shape::Shape(const Mat &_mat)
{
  _mat.copyTo(this->mat);
}

vector<Point2d> Shape::toPoints() const
{
  vector<Point2d> vs;
  int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    vs.push_back(Point2d(this->mat.at<double>(j,0), this->mat.at<double>(j,1)));
  }
  return vs;
}

const Point2d& Shape::centroid() const
{
  Mat m;
  reduce(this->mat, m, 0, CV_REDUCE_AVG); // Mean by row
  return Point2d(m.at<double>(0,0), m.at<double>(0,1));
}

vector<Point2d> Shape::convexHull() const
{
  vector<Point2d> hull;
  cv::convexHull(Mat(this->toPoints()), hull, false);
  return hull;
}

const double Shape::sumSquareDistanceToPoint(const Point2d& p) const
{
  // Procrustes distance
  double d = 0;
  int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    double x = this->mat.at<double>(j,0);
    double y = this->mat.at<double>(j,1);
    d += Aux::square(y - p.y) + Aux::square(x - p.x);
  }
  return d;
}

Shape Shape::operator-(const Shape& another) const
{
  // Square distance of the two shape (element-wise)
  this->mat -= another.mat;
  return Shape(this->mat.mul(this->mat));
}

Shape Shape::operator+(const Shape& another) const
{
  return Shape(this->mat + another.mat);
}

Shape Shape::operator*(double scale) const
{
  this->mat *= scale;
}

Shape Shape::operator >>(Point2d shift) const
{
  int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    this->mat.at<double>(j,0) += shift.x;
    this->mat.at<double>(j,1) += shift.y;
  }
}

void Shape::save(const string path) const
{

}

void Shape::load(const string path)
{
  
}




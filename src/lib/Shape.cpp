#include "Shape.h"

Shape::Shape(const vector<Point2d>& vs)
{
  this->mat = Mat(vs.size(), 2, CV_32FC1);
  for (int j=0; j<vs.size(); j++)
  {
    this->mat.at<float>(j,0) = vs[0].x;
    this->mat.at<float>(j,1) = vs[0].y;
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
    vs.push_back(Point2d(this->mat.at<float>(j,0), this->mat.at<float>(j,1)));
  }
  return vs;
}

const Point2d& Shape::centroid() const
{
  Mat m;
  reduce(this->mat, m, 0, CV_REDUCE_AVG); // Mean by row
  return Point2d(m.at<float>(0,0), m.at<float>(0,1));
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
    double x = this->mat.at<float>(j,0);
    double y = this->mat.at<float>(j,1);
    d += Aux::square(y - p.y) + Aux::square(x - p.x);
  }
  return d;
}

Shape Shape::operator-(const Shape& another) const
{
  this->mat -= another.mat;
  int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    auto x = this->mat.at<float>(j,0);
    auto y = this->mat.at<float>(j,1);
    this->mat.at<float>(j,0) = Aux::sqrt(Aux::square(x));
    this->mat.at<float>(j,1) = Aux::sqrt(Aux::square(y));
  }
  return Shape(mat_);
}

Shape Shape::operator+(const Shape& another) const
{
  vector<Point2d> sum;
  for (int i=0; i<this->vertices.size(); i++)
  {
    sum.push_back(this->vertices[i] + another.vertices[i]);
  }
  return Shape(sum);
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
    this->mat.at<float>(j,0) += shift.x;
    this->mat.at<float>(j,1) += shift.y;
  }
}

void Shape::save(const string path) const
{

}

void Shape::load(const string path)
{
  
}




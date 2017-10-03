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

Point2d Shape::centroid() const
{
  Mat m;
  reduce(this->mat, m, 0, CV_REDUCE_AVG); // Mean by row
  return Point2d(m.at<double>(0,0), m.at<double>(0,1));
}

/**
 * Compute the sum of square distance to another shape.
 */
const double Shape::procrustesDistance(const Shape& that) const
{
  double d = 0.0;
  int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    auto pThis = Point2d(this->mat.at<double>(j,0), this->mat.at<double>(j,1));
    auto pThat = Point2d(that.mat.at<double>(j,0), that.mat.at<double>(j,1));
    d += Aux::squareDistance(pThis, pThat);
  }
  return d;
}

Mat Shape::render(IO::GenericIO* io, Mat background, double scaleFactor, Point2d recentre) const
{
  // Render shape vertices
  int N = this->mat.rows;
  Mat canvas = background.clone();
  for (int j=0; j<N; j++)
  {
    double x = this->mat.at<double>(j,0) * scaleFactor + recentre.x;
    double y = this->mat.at<double>(j,1) * scaleFactor + recentre.y;
    Draw::drawSpot(canvas, Point2d(x, y), Scalar(230,0,0));
    if (j>0)
    {
      double x0 = this->mat.at<double>(j-1,0) * scaleFactor + recentre.x;
      double y0 = this->mat.at<double>(j-1,1) * scaleFactor + recentre.y;
      line(canvas, Point2d(x0, y0), Point2d(x, y), Scalar(200,0,0));
    }
    else
    {
      double x0 = this->mat.at<double>(N-1,0) * scaleFactor + recentre.x;
      double y0 = this->mat.at<double>(N-1,1) * scaleFactor + recentre.y;
      line(canvas, Point2d(x0, y0), Point2d(x, y), Scalar(200,0,0));
    }
  }

  io->render(canvas);
  return canvas;
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
  return Shape(this->mat * scale);
}

Shape Shape::operator >>(Point2d shift) const
{
  int N = this->mat.rows;
  Mat mat_ = this->mat.clone();
  for (int j=0; j<N; j++)
  {
    mat_.at<double>(j,0) += shift.x;
    mat_.at<double>(j,1) += shift.y;
  }
  return Shape(mat_);
}

Shape Shape::operator <<(Point2d shift) const
{
  int N = this->mat.rows;
  Mat mat_ = this->mat.clone();
  for (int j=0; j<N; j++)
  {
    mat_.at<double>(j,0) -= shift.x;
    mat_.at<double>(j,1) -= shift.y;
  }
  return Shape(mat_);
}

Mat Shape::toRowVector() const
{
  return this->mat.reshape(1, 1);
}

Mat Shape::toColVector() const
{
  int N = this->mat.rows;
  return this->mat.reshape(1, N*2);
}

Shape Shape::recentreAndScale(Point2d t, double scaleFactor) const
{
  return (*this * scaleFactor) >> t;
}

void Shape::save(const string path) const
{
  // TAOTODO:
}

void Shape::load(const string path)
{
  
}




#include "Triangle.h"

Triangle::Triangle(const vector<Point2d>& v)
{
  this->vertices = Mat(3, 2, CV_64FC1);
  for (int n=0; n<3; n++)
  {
    this->vertices.at<double>(n,0) = v[n].x;
    this->vertices.at<double>(n,1) = v[n].y;
  }
}

Triangle Triangle::operator >>(const Point2d &displacement) const
{
  Mat dist = Mat(3, 2, CV_64FC1);
  for (int i=0; i<3; i++)
  {
    dist.at<double>(i,0) = displacement.x;
    dist.at<double>(i,1) = displacement.y;
  }
  return Triangle(this->vertices + dist);
}

Triangle Triangle::operator <<(const Point2d &displacement) const
{
  Mat dist = Mat(3, 2, CV_64FC1);
  for (int i=0; i<3; i++)
  {
    dist.at<double>(i,0) = displacement.x;
    dist.at<double>(i,1) = displacement.y;
  }
  return Triangle(this->vertices - dist);
}

void Triangle::boundary(double& minX, double& minY, double& maxX, double& maxY) const
{
  minX = numeric_limits<double>::max();
  minY = numeric_limits<double>::max();
  maxY = -numeric_limits<double>::max();
  maxX = -numeric_limits<double>::max();
  for (int i=0; i<3; i++)
  {
    double x = this->vertices.at<double>(i,0);
    double y = this->vertices.at<double>(i,1);
    minX = x < minX ? x : minX;
    minY = y < minY ? y : minY;
    maxX = x > maxX ? x : maxX;
    maxY = y > maxY ? y : maxY;
  }
}

double Triangle::minX() const
{
  double a,b,c,d;
  boundary(a,b,c,d);
  return a;
}

double Triangle::minY() const
{
  double a,b,c,d;
  boundary(a,b,c,d);
  return b;
}

double Triangle::maxX() const
{
  double a,b,c,d;
  boundary(a,b,c,d);
  return c;
}

double Triangle::maxY() const
{
  double a,b,c,d;
  boundary(a,b,c,d);
  return d;
}

Rect Triangle::boundingRect() const
{
  double a,b,c,d;
  boundary(a,b,c,d);
  return Rect(a,b,c-a,d-b);
}

vector<Point2f> Triangle::toFloatVector() const
{
  vector<Point2f> v;
  for (int i=0; i<3; i++)
  {
    // Enforce double => float implication
    float x = vertices.at<double>(i,0);
    float y = vertices.at<double>(i,1);
    v.push_back(Point2f(x, y));
  }
  return v;
}

vector<Point2d> Triangle::toVector() const
{
  vector<Point2d> v;
  for (int i=0; i<3; i++)
  {
    double x = vertices.at<double>(i,0);
    double y = vertices.at<double>(i,1);
    v.push_back(Point2d(x, y));
  }
  return v;
}

void Triangle::toIntArray(Point* p) const
{
  for (int i=0; i<3; i++)
  {
    p[i].x = (int)vertices.at<double>(i,0);
    p[i].y = (int)vertices.at<double>(i,1);
  }
}
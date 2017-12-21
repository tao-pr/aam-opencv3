#include "Triangle.h"

// Triangle Triangle::operator >>(const Point2d &displacement) const
// {
//   Mat dist = Mat(3, 2, CV_64FC1);
//   for (int i=0; i<3; i++)
//   {
//     dist.at<double>(i,0) = displacement.x;
//     dist.at<double>(i,1) = displacement.y;
//   }
//   return Triangle(this->vertices + dist, this->ids);
// }

// Triangle Triangle::operator <<(const Point2d &displacement) const
// {
//   Mat dist = Mat(3, 2, CV_64FC1);
//   for (int i=0; i<3; i++)
//   {
//     dist.at<double>(i,0) = displacement.x;
//     dist.at<double>(i,1) = displacement.y;
//     dist.at<double>(i,2) = 0;
//   }
//   return Triangle(this->vertices - dist, this->ids);
// }

void Triangle::boundary(const Mat& m, double& minX, double& minY, double& maxX, double& maxY) const
{
  minX = numeric_limits<double>::max();
  minY = numeric_limits<double>::max();
  maxY = -numeric_limits<double>::max();
  maxX = -numeric_limits<double>::max();
  int vx[3] = {a, b, c};
  for (int i=0; i<3; i++)
  {
    double x = m.at<double>(vx[i],0);
    double y = m.at<double>(vx[i],1);
    minX = x < minX ? x : minX;
    minY = y < minY ? y : minY;
    maxX = x > maxX ? x : maxX;
    maxY = y > maxY ? y : maxY;
  }
}

Rect Triangle::boundingRect(const Mat& mat) const
{
  double a,b,c,d;
  boundary(mat,a,b,c,d);
  return Rect(a,b,c-a,d-b);
}

vector<Point2f> Triangle::toFloatVector(const Mat& mat) const
{
  vector<Point2f> v;
  int vx[3] = {a, b, c};
  for (int i=0; i<3; i++)
  {
    // Enforce double => float implication
    float x = mat.at<double>(vx[i],0);
    float y = mat.at<double>(vx[i],1);
    v.push_back(Point2f(x, y));
  }
  return v;
}

vector<Point2d> Triangle::toVector(const Mat& mat) const
{
  vector<Point2d> v;
  int vx[3] = {a, b, c};
  for (int i=0; i<3; i++)
  {
    double x = mat.at<double>(vx[i],0);
    double y = mat.at<double>(vx[i],1);
    v.push_back(Point2d(x, y));
  }
  return v;
}

void Triangle::toIntArray(Point* p, const Mat& mat) const
{
  int vx[3] = {a, b, c};
  for (int i=0; i<3; i++)
  {
    p[i].x = (int)mat.at<double>(vx[i],0);
    p[i].y = (int)mat.at<double>(vx[i],1);
  }
}
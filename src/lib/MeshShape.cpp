#include "MeshShape.h"

MeshShape::MeshShape(vector<Point2d>& vs) : Shape(vs)
{
  resubdiv();
}

MeshShape::MeshShape(const Mat& mat) : Shape(mat)
{
  resubdiv();
}

MeshShape::MeshShape(const MeshShape& original)
{
  MeshShape(original.mat);
}

MeshShape::MeshShape(const Shape& shape)
{
  MeshShape(shape.mat);
  resubdiv();
}

void MeshShape::resubdiv()
{
  this->subdiv = Subdiv2D();
  int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    this->subdiv.insert(Point2d(this->mat.at<double>(j,0), this->mat.at<double>(j,1)));
  }
}

Mat MeshShape::render(IO::GenericIO* io, Mat background, double scaleFactor) const
{
  // TAOREVIEW: Utilise OpenGL
  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);
  vector<Point2d> hull = this->convexHull();
  Size size = background.size();
  Mat canvas = Mat(size.height, size.width, CV_64FC3);
  background.copyTo(canvas);

  // Render edges
  for (auto tr : triangles)
  {
    auto a = Point2d(tr[0]*scaleFactor, tr[1]*scaleFactor);
    auto b = Point2d(tr[2]*scaleFactor, tr[3]*scaleFactor);
    auto c = Point2d(tr[4]*scaleFactor, tr[5]*scaleFactor);
    Draw::drawTriangle(canvas, a,b,c, Scalar(0,0,200), 1, CV_AA);
  }

  // Render boundary
  Point2d v0 = hull.front();
  Point2d v0s = Point2d(v0.x * scaleFactor, v0.y * scaleFactor);
  for (auto v : hull)
  {
    auto vs = Point2d(v.x * scaleFactor, v.y * scaleFactor);
    line(canvas, v0s, vs, Scalar(0,0,200), 3, CV_AA);
    v0s = vs;
  }

  io->render(canvas);
  return canvas;
}
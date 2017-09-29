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

Mat MeshShape::render(IO::GenericIO* io, Mat background) const
{
  // TAOREVIEW: Utilise OpenGL
  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);
  vector<Point2d> hull = this->convexHull();
  Size size = background.size;
  Mat canvas = Mat(size.height, size.width, CV_64FC3);
  background.copyTo(canvas);

  // Render edges
  for (auto tr : triangles)
  {
    auto a = Point2d(tr[0], tr[1]);
    auto b = Point2d(tr[2], tr[3]);
    auto c = Point2d(tr[4], tr[5]);
    Draw::drawTriangle(canvas, a,b,c, Scalar(0,0,200), 1, CV_AA);
  }

  // Render boundary
  Point2d v0 = hull.front();
  for (auto v : hull)
  {
    line(canvas, v0, v, Scalar(0,0,200), 3, CV_AA);
    v0 = v;
  }

  io->render(canvas);
  return canvas;
}
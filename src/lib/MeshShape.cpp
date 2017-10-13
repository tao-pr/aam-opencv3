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

int MeshShape::numShapes() const
{
  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);
  return triangles.size();
}

vector<Triangle> MeshShape::getTriangles() const
{
  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);
  vector<Triangle> output;
  for (auto tr:triangles)
  {
    vector<Point2d> pairs = {Point2d(tr[0], tr[1]), Point2d(tr[2], tr[3]), Point2d(tr[4], tr[5])};
    output.push_back(Triangle(pairs));
  }
  return output;
}

Mat MeshShape::render(IO::GenericIO* io, Mat background, double scaleFactor, Point2d recentre) const
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
    auto a = Point2d(tr[0]*scaleFactor + recentre.x, tr[1]*scaleFactor + recentre.y);
    auto b = Point2d(tr[2]*scaleFactor + recentre.x, tr[3]*scaleFactor + recentre.y);
    auto c = Point2d(tr[4]*scaleFactor + recentre.x, tr[5]*scaleFactor + recentre.y);
    Draw::drawTriangle(canvas, a,b,c, Scalar(0,0,200), 1, CV_AA);
  }

  // Render boundary
  Point2d v0 = hull.front();
  Point2d v0s = Point2d(v0.x * scaleFactor + recentre.x, v0.y * scaleFactor + recentre.y);
  for (auto v : hull)
  {
    auto vs = Point2d(v.x * scaleFactor + recentre.x, v.y * scaleFactor + recentre.y);
    line(canvas, v0s, vs, Scalar(0,0,200), 3, CV_AA);
    v0s = vs;
  }

  io->render(canvas);
  return canvas;
}
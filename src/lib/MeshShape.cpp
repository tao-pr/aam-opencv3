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
  double minX, minY, maxX, maxY;
  minMaxLoc(this->mat.col(0), &minX, &maxX);
  minMaxLoc(this->mat.col(1), &minY, &maxY);

  const double margin = 8.0;
  Rect rect((int)floor(minX-margin), 
    (int)floor(minY-margin), 
    (int)ceil(maxX-minX+margin*2), 
    (int)ceil(maxY-minY+margin*2));
  this->subdiv = Subdiv2D(rect);

  const int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    this->subdiv.insert(Point2f(
      (float)this->mat.at<double>(j,0), 
      (float)this->mat.at<double>(j,1)));
  }
}

int MeshShape::numTriangles() const
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

  // Render vertices
  Draw::drawSpots(canvas, this->toPoints(), Scalar(0,240,255));
  io->render(canvas);
  return canvas;
}
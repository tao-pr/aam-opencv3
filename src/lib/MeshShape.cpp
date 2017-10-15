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
  return this->getTriangles().size();
}

vector<Triangle> MeshShape::getTriangles() const
{
  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);

  // Take only triangles of which edges are aligned 
  // on or within the convex hull of the entire shape.
  auto hull = this->convexHull();
  Rect rect = boundingRect(hull);
  const Point* hulls[] = {hull.data()};
  const int counters[] = {(int)hull.size()};
  Mat hullFill = Mat::zeros(rect.height + rect.y, rect.width + rect.x, CV_8UC1);
  fillPoly(hullFill, hulls, counters, 1, Scalar(255), LINE_8);

  vector<Triangle> output;
  for (auto tr:triangles)
  {
    auto a = Point2d(tr[0], tr[1]);
    auto b = Point2d(tr[2], tr[3]);
    auto c = Point2d(tr[4], tr[5]);
    
    if (hullFill.at<unsigned char>(a.y, a.x) > 0 && 
        hullFill.at<unsigned char>(b.y, b.x) > 0 &&
        hullFill.at<unsigned char>(c.y, c.x) > 0)
    {
      vector<Point2d> pairs = {
        Point2d(a.x, a.y), 
        Point2d(b.x, b.y), 
        Point2d(c.x, c.y)};
      output.push_back(Triangle(pairs));
    }
  }
  return output;
}

Mat MeshShape::render(IO::GenericIO* io, Mat background, double scaleFactor, Point2d recentre) const
{
  // TAOREVIEW: Utilise OpenGL
  auto triangles = this->getTriangles();
  Size size = background.size();
  Mat canvas = Mat(size.height, size.width, CV_64FC3);
  background.copyTo(canvas);

  auto hull = this->convexHull();

  // Render edges
  for (auto tr : triangles)
  {
    const auto vec = tr.toVector();
    const auto a = vec[0];
    const auto b = vec[1];
    const auto c = vec[2];
    Draw::drawTriangle(canvas, a,b,c, Scalar(0,0,200), 1, CV_AA);
  }

  // Render convex hull
  Point prev = hull[0];
  for (auto next : hull)
  {
    line(canvas, prev, next, Scalar(0,0, 255), 3, CV_AA);
    prev = next;
  }
  line(canvas, prev, hull[0], Scalar(0,0, 255), 3, CV_AA);

  // Render vertices
  Draw::drawSpots(canvas, this->toPoints(), Scalar(0,240,255));
  io->render(canvas);
  return canvas;
}
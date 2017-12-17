#include "MeshShape.h"

MeshShape::MeshShape(const vector<Point2d>& vs) : Shape(vs)
{
  resubdiv();
}

MeshShape::MeshShape(const Mat& mat) : Shape(mat)
{
  resubdiv();
}

MeshShape::MeshShape(const MeshShape& original)
{
  this->mat = original.mat.clone();
  this->subdiv = original.subdiv;
}

MeshShape::MeshShape(const Shape& shape)
{
  MeshShape(shape.mat); // resubdiv is called automatically
}

void MeshShape::resubdiv()
{
  double minX, minY, maxX, maxY;
  minMaxLoc(this->mat.col(0), &minX, &maxX);
  minMaxLoc(this->mat.col(1), &minY, &maxY);

  const double margin = 8.0;
  this->bound = Rect((int)floor(minX-margin), 
    (int)floor(minY-margin), 
    (int)ceil(maxX-minX+margin*2), 
    (int)ceil(maxY-minY+margin*2));
  this->subdiv = Subdiv2D(bound);
  this->vertexToTriangles.clear();

  const int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    this->vertexToTriangles.insert(make_pair(j, vector<int>()));
    this->subdiv.insert(Point2f(
      (float)this->mat.at<double>(j,0), 
      (float)this->mat.at<double>(j,1)));
  }
  // TAOTODO: Show the mapping of vertex when on debug mode
}

int MeshShape::numTriangles() const
{
  return this->getTriangles().size();
}

Mat MeshShape::convexFill() const
{
  auto hull = this->convexHull();
  Rect rect = boundingRect(hull);
  const Point* hulls[] = {hull.data()};
  const int counters[] = {(int)hull.size()};
  Mat hullFill = Mat::zeros(rect.height + rect.y, rect.width + rect.x, CV_8UC1);
  fillPoly(hullFill, hulls, counters, 1, Scalar(255), LINE_8);  
  return hullFill;
}

const int MeshShape::findIndex(const Point2d& p) const
{
  int rows = this->mat.rows;
  for (int j=0; j<rows; j++)
  {
    if (this->mat.at<double>(j,0) == p.x &&
      this->mat.at<double>(j,1) == p.y)
      return j;
  }
  throw new domain_error("The point is not locatable inside the current shape");
}

vector<Triangle> MeshShape::getTriangles() const
{
  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);

  Mat hullFill = this->convexFill();
  vector<Triangle> output;
  priority_queue<SumIndicesToTriangle, vector<SumIndicesToTriangle>,SumIndicesCompare> q;
  for (auto tr:triangles)
  {
    auto a = Point2d(tr[0], tr[1]);
    auto b = Point2d(tr[2], tr[3]);
    auto c = Point2d(tr[4], tr[5]);
    
    if (hullFill.at<unsigned char>(a.y, a.x) > 0 && 
        hullFill.at<unsigned char>(b.y, b.x) > 0 &&
        hullFill.at<unsigned char>(c.y, c.x) > 0)
    {
      vector<Point2d> pairs = {a,b,c};
        // Point2d(a.x, a.y), 
        // Point2d(b.x, b.y), 
        // Point2d(c.x, c.y)};
      //output.push_back(Triangle(pairs));
      int ai = findIndex(a); // TAOTOREVIEW: This operation takes O(N) in the worst case
      int bi = findIndex(b);
      int ci = findIndex(c);
      int sumIndices = ai + bi + ci;
      q.push(make_tuple(sumIndices, Triangle(pairs)));
    }
  }

  // Convert the ordered triangles 
  while (!q.empty())  
  {
    output.push_back(get<1>(q.top()));
    q.pop();
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

void MeshShape::moveVertex(int i, const Point2d& displacement)
{
  auto oldVertex = Point2d(mat.at<double>(i,0), mat.at<double>(i,1));
  Shape::moveVertex(i, displacement);
  
  // TAOTODO: Re-adjust the subdiv2d
}
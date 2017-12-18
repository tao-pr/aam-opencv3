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
  this->trianglesCache = original.trianglesCache;
  this->mapVertexToTriangles = original.mapVertexToTriangles;
}

MeshShape::MeshShape(const Shape& shape)
{
  MeshShape(shape.mat); // resubdiv is called automatically
}

const bool MeshShape::isInside(const Point2d& p) const
{
  return p.x >= bound.x &&
    p.x < bound.x + bound.width && 
    p.y >= bound.y && 
    p.y < bound.y + bound.height;
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

  #ifdef DEBUG
  cout << "MeshShape::resubdiv ~ " << this->bound << endl;
  #endif

  this->subdiv = Subdiv2D(bound);

  const int N = this->mat.rows;
  for (int j=0; j<N; j++)
  {
    this->subdiv.insert(Point2f(
      (float)this->mat.at<double>(j,0), 
      (float)this->mat.at<double>(j,1)));
  }

  repopulateCache();
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
  const double PRECISION = 1e-5;
  int rows = this->mat.rows;
  for (int j=0; j<rows; j++)
  {
    if (abs(this->mat.at<double>(j,0) - p.x) <= PRECISION &&
      abs(this->mat.at<double>(j,1) - p.y) <= PRECISION)
      {
        return j;
      }
  }
  #ifdef DEBUG
  cout << RED << p << " can't be located in the following Matrix" << RESET << endl;
  cout << this->mat << endl;
  #endif
  throw new domain_error("The point is not locatable inside the current shape");
}

void MeshShape::addVertexMap(int vi, int ti)
{
  vector<int> empty;
  while (this->mapVertexToTriangles.size()<=vi) this->mapVertexToTriangles.push_back(empty);
  this->mapVertexToTriangles[vi].push_back(ti);
}

void MeshShape::repopulateCache()
{
  #ifdef DEBUG
  cout << "MeshShape::repopulateCache" << endl;
  #endif

  this->trianglesCache.clear();
  this->mapVertexToTriangles.clear();

  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);

  Mat hullFill = this->convexFill();
  int ti = 0;
  for (auto tr:triangles)
  {
    auto a = Point2d(tr[0], tr[1]);
    auto b = Point2d(tr[2], tr[3]);
    auto c = Point2d(tr[4], tr[5]);
    
    // NOTE: the vertex which lies outside of the visual boundary 
    //      will not be taken into consideration
    if (isInside(a) && isInside(b) && isInside(c) &&
        hullFill.at<unsigned char>(a.y, a.x) > 0 && 
        hullFill.at<unsigned char>(b.y, b.x) > 0 &&
        hullFill.at<unsigned char>(c.y, c.x) > 0)
    {
      vector<Point2d> pairs = {a,b,c};
      int ai = findIndex(a); // TAOTOREVIEW: This operation takes O(N) in the worst case
      int bi = findIndex(b);
      int ci = findIndex(c);
      trianglesCache.push_back(Triangle(pairs, ai, bi, ci));
      addVertexMap(ai, ti);
      addVertexMap(bi, ti);
      addVertexMap(ci, ti);
      ++ti;
    }
  }

  #ifdef DEBUG
  cout << this->trianglesCache.size() << " triangles stored in cache" << endl;
  cout << this->mapVertexToTriangles.size() << "vertices stored in cache" << endl;
  cout << CYAN << "[Vertex -> Triangle]" << RESET << endl;
  int n = 0;
  for (auto vm : mapVertexToTriangles)
  {
    cout << "v# " << n  << " ~~ ";
    for (auto v : vm) 
      cout << v << ",";
    cout << endl;
    ++n;
  }
  #endif
}

Mat MeshShape::render(IO::GenericIO* io, Mat background, double scaleFactor, Point2d recentre) const
{
  // TAOREVIEW: Utilise OpenGL
  auto triangles = this->getTriangles();
  Size size = background.size();
  Mat canvas = Mat(size.height, size.width, CV_64FC3);
  background.copyTo(canvas);

  auto hull = this->convexHull();

  // TAODEBUG:
  cout << "Triangles size : " << triangles.size() << endl;

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
  assert(i < this->mapVertexToTriangles.size());
  Shape::moveVertex(i, displacement);
  // Update affected triangles
  for (auto ti : this->mapVertexToTriangles[i])
  {
    for (int k=0; k<3; k++)
    {
      if (this->trianglesCache[ti].ids.at<int>(k,0)==i)
      {
        this->trianglesCache[ti].vertices.at<double>(k,0) += displacement.x;  
        this->trianglesCache[ti].vertices.at<double>(k,1) += displacement.y; 
        break;
      }
    }
  }
}

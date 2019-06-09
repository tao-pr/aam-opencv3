#include "MeshShape.h"

ostream &operator<<(ostream &os, MeshShape const &m)
{
  return os << "[MeshShape] bound : " << m.getBound() << ", " << m.numTriangles() << " triangles";
}

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
  this->bound = original.bound;
  this->trianglesCache = original.trianglesCache;
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

  const double margin = 2.0;
  this->bound = Rect((int)floor(minX-margin), 
    (int)floor(minY-margin), 
    (int)ceil(maxX-minX+margin*2), 
    (int)ceil(maxY-minY+margin*2));

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
  const double PRECISION = 1e-3;
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

void MeshShape::repopulateCache()
{
  this->trianglesCache.clear();

  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);

  struct CompareTripletIndex
  {
    // Ascending order
    inline bool operator()(Triangle &t1, Triangle &t2)
    { 
      long k1 = t1.a * 1000 + t1.b * 666 + t1.c;
      long k2 = t2.a * 1000 + t2.b * 666 + t2.c;
      return k1 > k2;
    }
  };

  priority_queue<Triangle, vector<Triangle>, CompareTripletIndex> q;

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
      int ai = findIndex(a); // TAOTOREVIEW: This operation takes O(N) in the worst case
      int bi = findIndex(b);
      int ci = findIndex(c);
      q.push(Triangle(ai, bi, ci));
      ++ti;
    }
  }

  while (!q.empty())
  {
    trianglesCache.push_back(q.top());
    q.pop();
  }
}

Mat MeshShape::render(IO::GenericIO* io, Mat background, double scaleFactor, Point2d recentre) const
{
  // TAOREVIEW: Utilise OpenGL
  auto triangles = this->getTriangles();
  Size size = background.size();
  Mat canvas = Mat(size.height, size.width, CV_64FC3);
  background.copyTo(canvas);

  auto hull = this->convexHull();

  #ifdef DEBUG
  cout << "MeshShape::render : num triangles = " << triangles.size() << endl;
  #endif

  // Render edges
  for (auto tr : triangles)
  {
    const auto vec = tr.toVector(this->mat);
    const auto a = vec[0];
    const auto b = vec[1];
    const auto c = vec[2];
    Draw::drawTriangle(canvas, a,b,c, Scalar(0,0,200), 1, LINE_AA);
  }

  // Render convex hull
  Point prev = hull[0];
  for (auto next : hull)
  {
    line(canvas, prev, next, Scalar(0,0, 255), 3, LINE_AA);
    prev = next;
  }
  line(canvas, prev, hull[0], Scalar(0,0, 255), 3, LINE_AA);

  // Render vertices
  Draw::drawSpots(canvas, this->toPoints(), Scalar(0,240,255));
  io->render(canvas);
  return canvas;
}

void MeshShape::moveVertex(int i, const Point2d& displacement)
{
  Shape::moveVertex(i, displacement);
}

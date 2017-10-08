#include "Texture.h"

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

void Texture::save(const string path) const
{

}

void Texture::load(const string path)
{

}

Mat Texture::render(IO::GenericIO* io, Mat background, bool withVertices, bool withEdges, double scaleFactor, Point2d recentre) const
{
  Mat canvas = background.clone();
  double a,b,c,d;
  this->bound.boundary(a,b,c,d);
  const vector<Point2d> vertices = this->bound.toVector();

  for (int i=(int)a; i<(int)c; i++)
    for (int j=(int)b; j<(int)d; j++)
    {
      if (Aux::isInsideShape(vertices, Point2d(i,j)))
      {
        canvas.at<Scalar>(j,i) = this->img->at<Scalar>(j,i);
      }
    }

  if (withEdges) Draw::drawTriangle(canvas, vertices[0], vertices[1], vertices[2], Scalar(0,235,200));
  if (withVertices) Draw::drawSpots(canvas, vertices, Scalar(0,255,220));

  // TAOTODO:
  io->render(canvas);
  return canvas;
}

/**
 * Piece-wise affine transformation.
 * Warping the texture to a new triangular boundary.
 */
Texture Texture::realignTo(const Triangle &newBound, Mat* dest) const
{
  assert(this->img->type() == dest->type());
  double minX, minY, maxX, maxY;
  newBound.boundary(minX, minY, maxX, maxY);

  Mat R( 2, 3, CV_64FC1 );
  auto srcTriangle  = this->bound.toFloatVector();
  auto destTriangle = newBound.toFloatVector();
  Rect srcRect      = boundingRect(srcTriangle);
  Rect destRect     = boundingRect(destTriangle);
  Size srcSize      = Size(srcRect.width, srcRect.height);
  Size destSize     = Size(destRect.width, destRect.height);

  // Crop the source by the bounding rectangle
  Mat im = *this->img;
  Mat imgSrc = Mat(srcSize, this->img->type());
  Mat imgDest = Mat::zeros(destSize, this->img->type());
  im(srcRect).copyTo(imgSrc);

  // Offset the triangles by left corner
  vector<Point2f> offsetSrcTriangle;
  vector<Point2f> offsetDestTriangle;
  Point* triangle = new Point[3]; // For convex drawing
  for (int i=0; i<3; i++)
  {
    auto pSrc  = srcTriangle[i];
    auto pDest = destTriangle[i];
    double xDest = pDest.x - destRect.x;
    double yDest = pDest.y - destRect.y;
    offsetSrcTriangle.push_back(Point2f(pSrc.x - srcRect.x, pSrc.y - srcRect.y));
    offsetDestTriangle.push_back(Point2f(xDest, yDest));
    triangle[i] = Point((int)xDest, (int)yDest);
  }

  // Apply affine transformation on the offset triangles
  Mat W = getAffineTransform(offsetSrcTriangle, offsetDestTriangle);
  warpAffine(imgSrc, imgDest, W, imgDest.size(), INTER_LINEAR);

  // Draw a triangular mask (for the target)
  Mat mask = Mat::zeros(destSize, CV_8UC1);
  const int counters[] = {3};
  const Point* triangles[] = {&triangle[0], &triangle[0]+3};
  fillPoly(mask, triangles, counters, 1, Scalar(255), LINE_8);

  // Clone pixels inside the mask to the output canvas
  for (int x=0; x<destRect.width; x++)
    for (int y=0; y<destRect.height; y++)
    {
      if (mask.at<int>(y,x) > 0)
      {
        dest->at<Scalar>(y+destRect.y, x+destRect.x) = imgDest.at<Scalar>(y,x);
      }
    }

  delete[] triangle;
  return Texture(newBound, dest);
}
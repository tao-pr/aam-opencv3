#include "Texture.h"

void Texture::save(const string path) const
{

}

void Texture::load(const string path)
{

}

Mat Texture::render(IO::GenericIO* io, Mat background, bool withVertices, bool withEdges, double scaleFactor, Point2d recentre) const
{
  assert(this->vertexRef != nullptr);
  Mat canvas = background.clone();
  double a,b,c,d;
  this->bound.boundary(*this->vertexRef,a,b,c,d);
  const vector<Point2d> vertices = this->bound.toVector(*this->vertexRef);

  Rect boundary((int)floor(a), (int)floor(b), (int)ceil(c), (int)ceil(d));

  // Draw the masking region
  auto triangle = new Point[3];
  this->bound.toIntArray(triangle,*this->vertexRef);
  Mat mask = Mat::zeros(boundary.height, boundary.width, CV_8UC1);
  const int counters[] = {3};
  const Point* triangles[] = {&triangle[0], &triangle[0]+3};
  fillPoly(mask, triangles, counters, 1, Scalar(255), LINE_8);

  for (int i=boundary.x; i<boundary.x + boundary.width; i++)
    for (int j=boundary.y; j<boundary.y + boundary.height; j++)
    {
      if (i>=0 && i<min(canvas.cols, img->cols) &&
          j>=0 && j<min(canvas.rows, img->rows) &&
          i<mask.cols && j<mask.rows &&
          mask.at<unsigned char>(j,i) > 0)
      {
        canvas.at<Vec3b>(j,i) = this->img->at<Vec3b>(j,i);
      }
    }

  if (withEdges) Draw::drawTriangle(canvas, vertices[0], vertices[1], vertices[2], Scalar(0,235,200));
  if (withVertices) Draw::drawSpots(canvas, vertices, Scalar(0,255,220));

  delete[] triangle;

  io->render(canvas);
  return canvas;
}

/**
 * Piece-wise affine transformation.
 * Warping the texture to a new triangular boundary.
 */
Texture Texture::realignTo(const Triangle &newBound, Mat* newVertexRef, Mat* dest) const
{
  assert(this->img->type() == dest->type());
  double minX, minY, maxX, maxY;
  newBound.boundary(*newVertexRef,minX, minY, maxX, maxY);

  Mat R( 2, 3, CV_64FC1 );
  auto srcTriangle  = this->bound.toFloatVector(*this->vertexRef);
  auto destTriangle = newBound.toFloatVector(*newVertexRef);
  Rect srcRect      = boundingRect(srcTriangle);
  Rect destRect     = boundingRect(destTriangle);
  Size srcSize      = Size(srcRect.width, srcRect.height);
  Size destSize     = Size(destRect.width, destRect.height);

  #ifdef DEBUG
  cout << "[Re-aligning texture]" << endl;
  cout << "... " << srcTriangle << endl;
  cout << "[to]" << endl;
  cout << "... " << destTriangle << endl << endl;
  #endif

  // Make sure the ROI is not exceeding the size of the image
  srcRect.width = min(srcRect.width, this->img->cols - srcRect.x);
  srcRect.height = min(srcRect.height, this->img->rows - srcRect.y);

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
  for (int x=0; x<destSize.width; x++)
    for (int y=0; y<destSize.height; y++)
    {
      if (mask.at<unsigned char>(y,x) > 0 && 
        x >= 0 && x < imgDest.cols &&
        y >= 0 && y < imgDest.rows &&
        x+destRect.x >= 0 && x+destRect.x < dest->cols &&
        y+destRect.y >= 0 && y+destRect.y < dest->rows)
      {
        dest->at<Vec3b>(y+destRect.y, x+destRect.x) = imgDest.at<Vec3b>(y,x);
      }
    }

  delete[] triangle;
  return Texture(newBound, newVertexRef, dest);
}
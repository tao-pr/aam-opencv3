#include "Shape.h"

Shape::Shape(const Rect& size)
{
  this->constraint = size;
  this->subdiv     = Subdiv2D(size);
}

Shape::Shape(const Rect& size, const vector<Point2d>& vs)
{
  this->constraint = size;
  this->vertices   = vs;
  this->subdiv     = Subdiv2D(size);
  for (auto const v : this->vertices)
  {
    this->subdiv.insert(v);
  }
}

Shape::Shape(const Shape& original)
{
  this->constraint = original.constraint;
  this->vertices   = original.vertices;
  this->subdiv     = Subdiv2D(size);
  for (auto const v : this->vertices)
  {
    this->subdiv.insert(v);
  }
}

Shape Shape::operator +(const Shape& that) const
{
  vector<Appearance> aOut;
  auto bi = that.a.begin();
  for (auto ai : this->a)
  {
    aOut.push_back(ai + *bi);
    ++bi;
  }
  return Shape(aOut);
}

Shape Shape::operator *(const double scale) const
{
  Rect newSize(
    (int)ceil(this->constraint.width * scale), 
    (int)ceil(this->constraint.height * scale));
  vector<Appearance> aOut;
  for (auto ai : this->a)
  {
    aOut.push_back(ai * scale);
  }
  return Shape(aOut);
}

void Shape::display(const string& wndName, double scaleFactor) const
{
  // TAOTOREVIEW: Utilise OpenGL
  Rect scaledBound = Rect(
    (int)ceil(scaleFactor * this->constraint.x),
    (int)ceil(scaleFactor * this->constraint.y),
    (int)ceil(scaleFactor * this->constraint.width),
    (int)ceil(scaleFactor * this->constraint.height)
  );
  vector<Vec6f> triangles;
  this->subdiv.getTriangleList(triangles);
  Mat canvas = Mat::zeros(scaledBound.height, scaledBound.width, CV_32FC3);
  for (int y=scaledBound.y; y<scaledBound.y + scaledBound.height; y++)
    for (int x=scaledBound.x; x<scaledBound.x + scaledBound.width; x++)
    {
      // TAOTODO:
    }

  imshow(canvas, wndName);
}

vector<Point> Shape::convexHull() const
{
  vector<Point> hull;
  cv::convexHull(Mat(this->vertices), hull, false);
  return hull;
}

void Shape::applyParameters(const vector<double>& params)
{
  // TAOTODO:
}

void Shape::resize(const Rect& newSize)
{
  double wScale = (double)newSize.width / this->constraint.width;
  double hScale = (double)newSize.height / this->constraint.height;
  this->constraint = newSize;

  // Regenerate the subdivision
  Subdiv2D newsubdiv(newSize);
  for (auto v : this->vertices)
  {
    v.x *= wScale;
    v.y *= hScale;
    this->subdiv.insert(v);
  }
  this->subdiv = newsubdiv;

  // Scale the appearance image
  Mat newApp(newSize, this->app.type());
  resize(this->app, newApp, newSize);
  newApp.copyTo(this->app);
}

void Shape::setAppearance(const Mat& src)
{
  this->app.copyFrom(src);
}

void Shape::normalise()
{
  this->resize(Rect(1.0, 1.0));
}
#include "Shape.h"


Shape::Shape(const vector<Point2d>& vs)
{
  this->vertices = vs;
}

Shape::Shape(const Shape& original)
{
  this->vertices   = original.vertices;
  this->subdiv     = Subdiv2D(size);
  for (auto const v : this->vertices)
  {
    this->subdiv.insert(v);
  }
}

const Point2d& Shape::meanPos() const
{
  double x = 0;
  double y = 0;
  double n = (double)this->vertices.size();
  for (auto p : this->vertices)
  {
    x += p.x;
    y += p.y;
  }
  return Point2d(x/n, y/n);
}

// void Shape::render(IO::GenericIO io, Mat background) const
// {
//   // TAOTOREVIEW: Utilise OpenGL
//   vector<Vec6f> triangles;
//   this->subdiv.getTriangleList(triangles);
//   vector<Point2d> hull = this->convexHull();
//   Mat canvas = Mat(scaledBound.height, scaledBound.width, CV_32FC3);
//   canvas.copyFrom(background);
//   // for (int y=scaledBound.y; y<scaledBound.y + scaledBound.height; y++)
//   //   for (int x=scaledBound.x; x<scaledBound.x + scaledBound.width; x++)
//   //   {

//   //   }

//   Point2d v0 = hull.front();
//   for (auto v : hull)
//   {
//     line(canvas, v0, v, Scalar(0,0,200), 1, CV_AA);
//     v0 = v;
//   }

//   io.render(canvas);
// }

vector<Point2d> Shape::convexHull() const
{
  vector<Point2d> hull;
  cv::convexHull(Mat(this->vertices), hull, false);
  return hull;
}

void Shape::resize(const Size& newSize)
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

// Shape& Shape::fromAnnotation(const vector<Point2d>& ps, Mat img)
// {
//   // Apply PCA on annotated points
//   Mat rawPoints = Mat(ps.size(), 2, CV_64FC1);
//   for (int i=0; i<ps.size(); ++i)
//   {
//     rawPoints.at(i, 0) = ps[i].x;
//     rawPoints.at(i, 1) = ps[i].y;
//   }

//   int nComponents = min(MAX_SHAPE_COMPONENTS, ps.size()/2);
//   PCA pca(rawPoints, Mat(), PCA::DATA_AS_COLUMN, nComponents);
//   Mat pcaPoints = Mat(nComponents, 2, CV_64FC1);

//   // TAOTODO:
// }

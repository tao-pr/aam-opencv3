/**
 * Generic Shape Model
 */

#ifndef SHAPE_MODEL
#define SHAPE_MODEL

#include "master.h"
#include "IO.h"
#include "aux.h"

class Shape
{
private:
  const int MAX_SHAPE_COMPONENTS = 16;

protected:
  vector<Point2d> vertices; // Triangulated mesh vertices
  Mat app; // Appearance image

public:
  Shape(){};
  Shape(vector<Point2d>& vs);
  Shape(const Shape& original);
  virtual ~Shape(){};

  size_t length() const { return this->vertices.size(); };
  const Point2d& meanXY() const;

  void save(const string path) const;
  void load(const string path);

  void setAppearance(const Mat& src);
  void setShapePCA(const Mat& src);
  void resize(const Size& newSize);
  vector<Point2d> convexHull() const;

  // Render the shape onto a background image
  // into an IO object.
  // void render(IO::GenericIO io, Mat background, double scaleFactor = 1.0) const;
};



#endif
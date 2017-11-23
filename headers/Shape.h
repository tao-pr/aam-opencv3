/**
 * Generic Shape Model
 */

#ifndef SHAPE_MODEL
#define SHAPE_MODEL

#include "master.h"
#include "BaseModel.h"
#include "IO.h"
#include "aux.h"

class Shape : public BaseModel
{
private:
public:
  Mat mat;

  inline Shape(){};
  Shape(const vector<Point2d>& vs);
  Shape(const Mat &mat);
  inline Shape(const Shape& original){ original.mat.copyTo(mat); };
  inline virtual ~Shape(){};

  //----- General properties ------
  Mat getMat() const { return this->mat; };
  Mat toRowVector() const;
  Mat toColVector() const;
  Point2d centroid() const;
  vector<Point> convexHull() const;
  const double sumSquareDistanceToPoint(const Point2d& p) const;
  const double procrustesDistance(const BaseModel* that) const;

  //------ I/O ------
  unique_ptr<BaseModel> clone() const;
  virtual void save(const string path) const;
  virtual void load(const string path);
  vector<Point2d> toPoints() const;
  virtual Mat render(IO::GenericIO* io, Mat background, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;

  //------ Operators -------
  Shape operator-(const Shape& another) const; // Displacement between two shapes
  Shape operator+(const Shape& another) const; // Addition of two shapes
  Shape operator*(double scale) const; // Scaling
  Shape operator >>(Point2d shift) const;  // Translating
  Shape operator <<(Point2d shift) const;  // Translating (negative)
  Shape recentreAndScale(Point2d t, double scaleFactor) const;
  void addRandomNoise(const Point2d& maxDisplacement);
};

#endif
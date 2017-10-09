/**
 * Generic Texture Model
 */

#ifndef TEXTURE_MODEL
#define TEXTURE_MODEL

#include "master.h"
#include "IO.h"
#include "aux.h"

class Triangle
{
public:
  Mat vertices; // 3 x 2
  Triangle(const vector<Point2d>& v);
  inline Triangle(const Mat& v) : vertices(v) {};
  inline ~Triangle(){};

  inline Point2d get(int i) const { return Point2d(vertices.at<double>(i,0), vertices.at<double>(i,1)); }
  void boundary(double& minX, double& minY, double& maxX, double& maxY) const;
  double minX() const;
  double minY() const;
  double maxX() const;
  double maxY() const;

  vector<Point2f> toFloatVector() const;
  vector<Point2d> toVector() const;
  Rect boundingRect() const;

  Triangle operator >>(const Point2d &displacement) const;
  Triangle operator <<(const Point2d &displacement) const;
};

/**
 * Texture coupled with a triangular face
 */
class Texture
{
private:
protected:
public:
  Mat *img;
  Triangle bound;

  inline Texture(const Triangle& bound, Mat* a) : img(a), bound(bound) {};
  inline Texture(const Texture& original) : img(original.img), bound(original.bound) {};
  virtual inline ~Texture(){};

  // --------- General properties -------


  // --------- I/O ------------
  virtual void save(const string path) const;
  virtual void load(const string path);
  virtual Mat render(IO::GenericIO* io, Mat background, bool withVertices=true, bool withEdges=true, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;

  //------ Operators / Transformations -------
  Texture realignTo(const Triangle &newBound, Mat* m) const;
};

#endif
/**
 * Subdivided Mesh
 */

#ifndef MESH_SHAPE 
#define MESH_SHAPE

#include "IO.h"
#include "aux.h"
#include "Shape.h"
#include "Triangle.h"

/**
 * Mapping from triangle [i] => associated group of vertices (by Ids in Mat)
 */
typedef vector<tuple<int,int,int>> SubdivPlan;

class MeshShape : public Shape 
{
private:
  void resubdiv();
  const int findIndex(const Point2d& p) const;
  const bool isInside(const Point2d& p) const;

protected:
  vector<Triangle> trianglesCache;
  SubdivPlan divCache;
  
  Subdiv2D subdiv;
  Rect bound;

public:
  MeshShape() : Shape(){};
  MeshShape(const vector<Point2d>& vs);
  MeshShape(const Mat& mat);
  MeshShape(const MeshShape& original);
  MeshShape(const Shape& shape);
  virtual inline ~MeshShape(){};

  int numTriangles() const;
  vector<Triangle> getTriangles() const;
  inline Rect getBound() const { return this->bound; };
  Mat convexFill() const;

  //------ I/O --------------
  Mat render(IO::GenericIO* io, Mat background, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;

  //------ Operators --------
  virtual void addRandomNoise(const Point2d& maxDisplacement);
};

#endif
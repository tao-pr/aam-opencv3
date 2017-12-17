/**
 * Subdivided Mesh
 */

#ifndef MESH_SHAPE 
#define MESH_SHAPE

#include "IO.h"
#include "aux.h"
#include "Shape.h"
#include "Triangle.h"

typedef tuple<int, Triangle> SumIndicesToTriangle;

class SumIndicesCompare
{
public:
  // Ascending order
  inline bool operator()(SumIndicesToTriangle &a, SumIndicesToTriangle &b)
  { 
    return get<0>(a) > get<0>(b); 
  }
};

class MeshShape : public Shape 
{
private:
  void resubdiv();
  const int findIndex(const Point2d& p) const;

protected:
  Subdiv2D subdiv;
  Rect bound;
  map<int, vector<int>> vertexToTriangles;

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
  virtual void moveVertex(int i, const Point2d& displacement);
};

#endif
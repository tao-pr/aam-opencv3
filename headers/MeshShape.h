/**
 * Subdivided Mesh
 */

#ifndef MESH_SHAPE 
#define MESH_SHAPE

#include "IO.h"
#include "aux.h"
#include "Shape.h"
#include "Triangle.h"

class MeshShape : public Shape 
{
private:
  void resubdiv();
  const int findIndex(const Point2d& p) const;
  const bool isInside(const Point2d& p) const;

protected:
  vector<Triangle> trianglesCache; // Triangle[i]

  Subdiv2D subdiv;
  Rect bound;

  void repopulateCache();
  void addVertexMap(int vi, int ti);

public:
  MeshShape() : Shape(){};
  MeshShape(const vector<Point2d>& vs);
  MeshShape(const Mat& mat);
  MeshShape(const MeshShape& original);
  MeshShape(const Shape& shape) : MeshShape(shape.mat){}; // This will also trigger [resubdiv] automatically
  virtual inline ~MeshShape(){};

  inline int numTriangles() const { return this->trianglesCache.size(); };
  inline const vector<Triangle>& getTriangles() const { return this->trianglesCache; };
  inline Rect getBound() const { return this->bound; };
  Mat convexFill() const;

  //------ I/O --------------
  Mat render(IO::GenericIO* io, Mat background, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;

  //------ Operators --------
  virtual void moveVertex(int i, const Point2d& displacement);
};

ostream &operator<<(ostream &os, MeshShape const &m);

#endif
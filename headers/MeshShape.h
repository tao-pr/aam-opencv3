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

protected:
  Subdiv2D subdiv;

public:
  MeshShape() : Shape(){};
  MeshShape(vector<Point2d>& vs);
  MeshShape(const Mat& mat);
  MeshShape(const MeshShape& original);
  MeshShape(const Shape& shape);
  virtual inline ~MeshShape(){};

  int numShapes() const;
  vector<Triangle> getTriangles() const;

  //------ I/O --------------
  Mat render(IO::GenericIO* io, Mat background, double scaleFactor=1.0, Point2d recentre=Point2d(0,0)) const;
};

#endif
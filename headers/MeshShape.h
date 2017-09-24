/**
 * Subdivided Mesh
 */

#ifndef MESH_SHAPE 
#define MESH_SHAPE

#include "IO.h"
#include "aux.h"
#include "Shape.h"

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
  virtual ~MeshShape();

  //------ I/O --------------
  void render(IO::GenericIO io, Mat background, double scaleFactor = 1.0) const;
};

#endif
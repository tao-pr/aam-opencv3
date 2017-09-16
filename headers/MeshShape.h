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
protected:
  Subdiv2D subdiv;
  
public:
  MeshShape();
  MeshShape(vector<Point2d>& vs);
  MeshShape(const MeshShape& original);
  virtual ~MeshShape();
};

#endif
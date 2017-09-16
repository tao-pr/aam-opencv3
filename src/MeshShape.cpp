#include "MeshShape.h"

MeshShape::MeshShape(vector<Point2d>& vs)
{
  Shape(vs);
  this->subdiv = Subdiv2D(size);
  for (auto const v : this->vertices)
  {
    this->subdiv.insert(v);
  }
}
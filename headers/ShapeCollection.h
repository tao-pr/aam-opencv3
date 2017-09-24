/**
 * Series of Shape for training process
 */

#ifndef SHAPE_COLLECTION
#define SHAPE_COLLECTION

#include "master.h"
#include "MeshShape.h"

class ShapeCollection 
{
private:
protected:
  vector<Shape> items;
public:
  inline ShapeCollection(){};
  ShapeCollection(const vector<Shape>& shapes);
  ShapeCollection(const ShapeCollection& original);

  inline void add(const Shape &s){ this->items.push_back(s); };

  Shape mean() const;
};




#endif
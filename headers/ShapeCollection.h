/**
 * Series of Shape for training process
 */

#ifndef SHAPE_COLLECTION
#define SHAPE_COLLECTION

#include "master.h"
#include "Shape.h"

class ShapeCollection 
{
private:
protected:
  vector<Shape> items;
public:
  inline ShapeCollection(){};
  inline ShapeCollection(const vector<Shape>& shapes) : items(shapes){};
  inline ShapeCollection(const ShapeCollection& original) : items(original.items){};

  inline void add(const Shape &s){ this->items.push_back(s); };

  Shape mean() const;
};




#endif
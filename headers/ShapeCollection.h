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
  bool verbose;
  vector<Shape> items;
public:
  inline ShapeCollection(const bool verbose=false) : verbose(verbose){};
  inline ShapeCollection(const vector<Shape>& shapes, const bool verbose=false) : verbose(verbose), items(shapes){};
  inline ShapeCollection(const ShapeCollection& original, const bool verbose=false) : verbose(verbose), items(original.items){};

  inline void add(const Shape &s){ this->items.push_back(s); };

  const ShapeCollection& normaliseScalingTranslation() const;
  const ShapeCollection& normaliseRotation() const;
};




#endif
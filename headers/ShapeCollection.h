/**
 * Series of Shape for training process
 */

#ifndef SHAPE_COLLECTION
#define SHAPE_COLLECTION

#include "IO.h"
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
  inline ~ShapeCollection(){};

  inline void add(const Shape &s){ this->items.push_back(s); };

  // ---------- Transformations ---------------
  ShapeCollection normaliseScalingTranslation() const;
  ShapeCollection normaliseRotation() const;
  ShapeCollection translateBy(const Point2d &p) const;

  // ---------- I/O ------------------
  void renderShapeVariation(IO::GenericIO* io, Size sz, double scaleFactor=1.0, Point2d recentred=Point2d(0,0)) const;
  vector<Shape> getItems() const;

};




#endif
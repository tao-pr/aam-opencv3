/**
 * Series of Shape for training process
 */

#ifndef SHAPE_COLLECTION
#define SHAPE_COLLECTION

#include "IO.h"
#include "master.h"
#include "Shape.h"
#include "Params.h"
#include "BaseModel.h"
#include "ModelCollection.h"

class ShapeCollection : public ModelCollection
{
public:
  inline ShapeCollection(const bool isVerbose=false) : verbose(isVerbose){};
  inline ShapeCollection(const vector<Shape*> shapes, const bool isVerbose=false);
  inline ShapeCollection(const ShapeCollection& original, const bool isVerbose=false) : verbose(isVerbose), items(original.items){};
  
  // ---------- Transformations ---------------
  ModelCollection clone(bool verbose=false) const;
  ShapeCollection normaliseScalingTranslation() const;
  ShapeCollection normaliseRotation() const;
  ShapeCollection translateBy(const Point2d &p) const;

  // ---------- I/O ------------------
  void renderShapeVariation(IO::GenericIO* io, Size sz, double scaleFactor=1.0, Point2d recentred=Point2d(0,0)) const;
  vector<Shape> getShapeItems() const;
  Mat toMat() const;

};




#endif
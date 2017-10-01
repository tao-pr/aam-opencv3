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
  inline ShapeCollection(const bool isVerbose=false) : verbose(isVerbose){};
  inline ShapeCollection(const vector<Shape>& shapes, const bool isVerbose=false) : verbose(isVerbose), items(shapes){};
  inline ShapeCollection(const ShapeCollection& original, const bool isVerbose=false) : verbose(isVerbose), items(original.items){};
  inline ~ShapeCollection(){};

  inline void add(const Shape &s){ this->items.push_back(s); };

  // ---------- Analysis of shapes ------------
  tuple<Shape, ShapeCollection> procrustesMeanShape(double tol=1e-3, int maxIter=10) const;
  double sumProcrustesDistance(const Shape& targetShape) const;
  Mat covariance(const Shape& mean) const;
  Shape pca(const Shape& meanShape) const;

  // ---------- Transformations ---------------
  ShapeCollection clone(bool verbose=false) const;
  ShapeCollection normaliseScalingTranslation() const;
  ShapeCollection normaliseRotation() const;
  ShapeCollection translateBy(const Point2d &p) const;

  // ---------- I/O ------------------
  void renderShapeVariation(IO::GenericIO* io, Size sz, double scaleFactor=1.0, Point2d recentred=Point2d(0,0)) const;
  vector<Shape> getItems() const;
  Mat toMat() const;

};




#endif
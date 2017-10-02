#ifndef PARAM_SHAPE
#define PARAM_SHAPE

#include "master.h"
#include "Shape.h"

class ShapeEncoder
{
protected:
  Mat mean;
  Mat eigen;
public:
  ShapeEncoder(mean, eigen) : mean(mean), eigen(eigen);
  virtual ~ShapeEncoder(){};
};

/**
 * Parameterised Shape, encoded by PCA
 */
class ParameterisedShape
{
private:
protected:
  Mat params;
public:
  ParameterisedShape(const Shape& shape);
  inline virtual ~ParameterisedShape(){};

  // ---------- Conversion -----------
  Shape toShape(const ShapeEncoder& enc) const;
};

#endif
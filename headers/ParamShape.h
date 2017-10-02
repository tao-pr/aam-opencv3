#ifndef PARAM_SHAPE
#define PARAM_SHAPE

#include "master.h"
#include "Shape.h"

class ParameterisedShape;

class ShapeEncoder
{
protected:
  Mat mean;
  Mat eigen;
  Mat eigen_1; // Invert of eigen
public:
  ShapeEncoder(Mat mean, Mat eigen) : mean(mean), eigen(eigen), eigen_1(eigen.inv()) {};
  virtual ~ShapeEncoder(){};
  Shape decode(const ParameterisedShape &s) const;
  Mat encode(const Shape& s) const;
};

/**
 * Parameterised Shape, encoded by PCA parameters
 */
class ParameterisedShape
{
private:
protected:
  Mat params;
public:
  ParameterisedShape(const Shape& shape, const ShapeEncoder& enc);
  inline virtual ~ParameterisedShape(){};

  Mat getParams() const { return this->params; };

  // ---------- Conversion -----------
  Shape toShape(const ShapeEncoder& enc) const;
};

#endif
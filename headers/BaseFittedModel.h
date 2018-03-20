#ifndef FITTED_MODEL
#define FITTED_MODEL

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"

class BaseFittedModel
{
protected:
  inline BaseFittedModel(){};
public:
  // Variable states
  Mat shapeParam;
  Mat appearanceParam;
  Point2d centre;
  double scale; // Scale multiplier (1x by default)

  inline BaseFittedModel(const ShapeModelPCA& pcaShape, const AppearanceModelPCA& pcaAppearance)
  {
    shapeParam = Mat::zeros(1, pcaShape.dimension(), CV_64FC1);
    appearanceParam = Mat::zeros(1, pcaAppearance.dimension(), CV_64FC1);
    this->centre = Point2d(0,0);
    this->scale = 1;
  };

  virtual BaseFittedModel* setCentre(const Point2d& p) = 0;
  virtual BaseFittedModel* setScale(const double s) = 0;
  virtual BaseFittedModel* setShapeParam(const Mat& param) = 0;
  virtual BaseFittedModel* setAppearanceParam(const Mat& param) = 0;

  virtual const double getMeanShapeScale() = 0;
  virtual Appearance* toAppearance() = 0;
  virtual MeshShape* toShape() = 0;
  virtual BaseFittedModel* clone() const = 0;

  virtual double measureError(const Mat& sample) = 0;
  virtual void drawOverlay(Mat& canvas) = 0;
};

inline ostream &operator<<(ostream &os, BaseFittedModel const &m) {
  return os << "FittedModel centered at : " << m.centre << endl
            << "            scale of    : " << m.scale << endl 
            << "            shape param : " << m.shapeParam.size() <<  endl
            << "            app param   : " << m.appearanceParam.size() << endl;
};


#endif
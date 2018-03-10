#ifndef FITTER
#define FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"
#include "ModelFitter.h"

/**
 * State of the active appearance model fitted onto a sample
 */
class FittedAAM 
{
private:
  FittedAAM() {};
protected:
  // Static model fitter instance (shared)
  ModelFitter* fitter; // TAOTODO: Use <shared_ptr> instead

public:
  // Variable states
  Mat shapeParam;
  Mat appearanceParam;
  Point2d centre;
  double scale; // Scale multiplier (1x by default)


  /**
   * Initialise a new AAM with mean shape and mean appearance
   */
  inline FittedAAM(ModelFitter* sharedFitter) : fitter(sharedFitter)
  {
    shapeParam = Mat::zeros(1, pcaShape().dimension(), CV_64FC1);
    appearanceParam = Mat::zeros(1, pcaAppearance().dimension(), CV_64FC1);
    centre = Point2d(0,0);
    scale = 1;
  };
  FittedAAM(const FittedAAM& another);
  virtual ~FittedAAM(){};

  const ShapeModelPCA pcaShape(){ return this->fitter->getShapePCA(); } const;
  const AppearanceModelPCA pcaAppearance(){ return this->fitter->getAppearancePCA(); } const;

  void setCentre(const Point2d& p);
  const double getMeanShapeScale();
  void setShapeParam(const Mat& param);
  void setAppearanceParam(const Mat& param);

  Appearance* toAppearance();
  MeshShape* toShape();

  double measureError(const Mat& sample);
  void drawOverlay(Mat& canvas);
};

#endif
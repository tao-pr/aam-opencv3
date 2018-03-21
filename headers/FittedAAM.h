#ifndef FITTER
#define FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"
#include "ModelFitter.h"
#include "BaseFittedModel.h"

/**
 * State of the active appearance model fitted onto a sample
 */
class FittedAAM : public BaseFittedModel
{
private:
  FittedAAM() : BaseFittedModel() {};

public:
  /**
   * Initialise a new AAM with mean shape and mean appearance
   */
  inline FittedAAM(shared_ptr<AAMPCA> const& aamPCA) : BaseFittedModel(aamPCA) {};
  
  FittedAAM(const FittedAAM& another) : BaseFittedModel(another){};
  virtual ~FittedAAM(){};

  const ShapeModelPCA pcaShape(){ return aamPCA->getShapePCA(); } const;
  const AppearanceModelPCA pcaAppearance(){ return aamPCA->getAppearancePCA(); } const;
  const double getMeanShapeScale();

  BaseFittedModel* setCentre(const Point2d& p);
  inline BaseFittedModel* setScale(const double s) { this->scale = s; return this; };
  BaseFittedModel* setShapeParam(const Mat& param);
  BaseFittedModel* setAppearanceParam(const Mat& param);

  Appearance* toAppearance();
  MeshShape* toShape();
  BaseFittedModel* clone() const;

  double measureError(const Mat& sample);
  void drawOverlay(Mat& canvas);
};

#endif
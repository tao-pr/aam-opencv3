#ifndef FITTER
#define FITTER

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"

class FittedAAM 
{
private:
  FittedAAM() {};
protected:
  // Static PCA of Shape and Appearance components
  ShapeModelPCA pcaShape;
  AppearanceModelPCA pcaAppearance;

public:
  // Variable states
  Mat shapeParam;
  Mat appearanceParam;
  Point2d centre;
  double scale;


  /**
   * Initialise a new AAM with mean shape and mean appearance
   */
  inline FittedAAM(const ShapeModelPCA& pcaShape, const AppearanceModelPCA& pcaAppearance)
    : pcaShape(pcaShape), pcaAppearance(pcaAppearance) 
      {
        shapeParam = Mat::zeros(1, pcaShape.dimension(), CV_64FC1);
        appearanceParam = Mat::zeros(1, pcaAppearance.dimension(), CV_64FC1);
        centre = Point2d(0,0);
        scale = 1;
      };
  FittedAAM(const FittedAAM& another);
  virtual ~FittedAAM(){};

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
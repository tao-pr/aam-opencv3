#ifndef FITTED_MODEL
#define FITTED_MODEL

class BaseFittedModel
{
public:
  // Variable states
  Mat shapeParam;
  Mat appearanceParam;
  Point2d centre;
  double scale; // Scale multiplier (1x by default)

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


#endif
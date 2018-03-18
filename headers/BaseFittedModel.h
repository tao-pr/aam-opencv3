#ifndef FITTED_MODEL
#define FITTED_MODEL

class BaseFittedModel
{
public:
  virtual void setCentre(const Point2d& p) = 0;
  virtual const double getMeanShapeScale() = 0;
  virtual void setShapeParam(const Mat& param) = 0;
  virtual void setAppearanceParam(const Mat& param) = 0;
  virtual BaseFittedModel* generateBestModel() = 0;

  virtual Appearance* toAppearance() = 0;
  virtual MeshShape* toShape() = 0;

  virtual double measureError(const Mat& sample) = 0;
  virtual void drawOverlay(Mat& canvas) = 0;
};


#endif
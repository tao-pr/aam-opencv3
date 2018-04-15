#ifndef FITTED_MODEL
#define FITTED_MODEL

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"

// TAOTODO: Destructor of this class causes issue when releasing [aamPCA]
class BaseFittedModel
{
protected:
  inline BaseFittedModel(){};
  inline BaseFittedModel(const BaseFittedModel& another)
  {
    this->aamPCA = another.aamPCA;
    another.shapeParam.copyTo(this->shapeParam);
    another.appearanceParam.copyTo(this->appearanceParam);
    this->origin = another.origin;
    this->scale = another.scale;
  };
  // Static model fitter instance (shared)
  shared_ptr<AAMPCA> aamPCA;

public:
  // Variable states
  Mat shapeParam;
  Mat appearanceParam;
  Point2d origin;
  double scale; // Scale multiplier (1x by default)

  inline BaseFittedModel(shared_ptr<AAMPCA> const& aamPCA) : aamPCA(aamPCA)
  {
    shapeParam = Mat::zeros(1, aamPCA->dimensionShape(), CV_64FC1);
    appearanceParam = Mat::zeros(1, aamPCA->dimensionAppearance(), CV_64FC1);
    this->origin = Point2d(0,0);
    this->scale = 1;
  };

  virtual BaseFittedModel* setOrigin(const Point2d& p) = 0;
  virtual BaseFittedModel* setScale(const double s) = 0;
  virtual BaseFittedModel* setShapeParam(const Mat& param) = 0;
  virtual BaseFittedModel* setAppearanceParam(const Mat& param) = 0;

  virtual const double getMeanShapeScale() const = 0;
  virtual Appearance* toAppearance() const = 0;
  virtual MeshShape* toShape() const = 0;
  virtual unique_ptr<BaseFittedModel> clone() const = 0;
  virtual Rect getBound() const = 0;

  virtual double measureError(const Mat& sample) = 0;
  virtual Mat drawOverlay(Mat& canvas, bool withEdges = false) = 0;
};

inline ostream &operator<<(ostream &os, BaseFittedModel const &m) {
  return os << "FittedModel centered at : " << m.origin << endl
            << "            scale of    : " << m.scale << endl 
            << "            shape param : " << m.shapeParam.size() <<  endl
            << "            app param   : " << m.appearanceParam.size() << endl;
};


#endif
#include "FittedAAM.h"

FittedAAM::FittedAAM(const FittedAAM& another)
{
  this->fitter = another.fitter;
  this->shapeParam = another.shapeParam; // TAOTOREVIEW: supposed to be `copyTo` instead?
  this->appearanceParam = another.appearanceParam;
  this->centre = another.centre;
}

void FittedAAM::setCentre(const Point2d& p)
{
  this->centre = p;
}

const double FittedAAM::getMeanShapeScale()
{
  return this->pcaAppearance().getScale();
}

void FittedAAM::setShapeParam(const Mat& param)
{
  param.copyTo(this->shapeParam);
}

void FittedAAM::setAppearanceParam(const Mat& param)
{
  param.copyTo(this->appearanceParam);
}

Appearance* FittedAAM::toAppearance()
{
  assert(this->scale > 0);

  auto app = this->pcaAppearance().toAppearance(this->appearanceParam);
  app->recentre(this->centre);
  
  if (this->scale != 1)
  {
    double scale0 = getMeanShapeScale();
    app->resizeTo(scale0 * this->scale);
  }
  return app;
}

MeshShape* FittedAAM::toShape()
{
  return this->pcaShape().toShape(this->shapeParam);
}

double FittedAAM::measureError(const Mat& sample)
{
  Appearance* app = this->toAppearance();
  int M = pcaAppearance().dimension();

  // Row vector representing the current state of appearance
  Mat selfRow = app->toRowVectorReduced(M);

  // Row vectot representing the current shape overlaid onto the sample
  app->setGraphic(sample);
  Mat sampleRow = app->toRowVectorReduced(M);

  return Aux::mse(sampleRow, selfRow);
}

void FittedAAM::drawOverlay(Mat& canvas)
{
  IO::MatIO m;
  Appearance* app = this->toAppearance();
  app->render(&m, canvas);
 
  // TAODEBUG:
  imshow("overlay", m.get());
  imshow("canvas", canvas);
  waitKey(0);
}

BaseFittedModel* FittedAAM::clone() const
{
  auto cloned = new FittedAAM(this->fitter);
  cloned->setCentre(this->centre);
  cloned->setShapeParam(this->shapeParam);
  cloned->setAppearanceParam(this->appearanceParam);
  return cloned;
}
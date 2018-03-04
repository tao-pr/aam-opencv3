#include "FittedAAM.h"

FittedAAM::FittedAAM(const FittedAAM& another)
{
  this->pcaShape = another.pcaShape;
  this->pcaAppearance = another.pcaAppearance;
  this->shapeParam = another.shapeParam;
  this->appearanceParam = another.appearanceParam;
  this->centre = another.centre;
}

void FittedAAM::setCentre(const Point2d& p)
{
  this->centre = p;
}

const double FittedAAM::getMeanShapeScale()
{
  return this->pcaAppearance.getScale();
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
  assert(this->scale != 0);

  auto app = this->pcaAppearance.toAppearance(this->appearanceParam);
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
  return this->pcaShape.toShape(this->shapeParam);
}

double FittedAAM::measureError(const Mat& sample)
{
  Appearance* app = this->toAppearance();
  int M = pcaAppearance.dimension();

  // Row vector representing the current state of appearance
  Mat selfRow = app->toRowVectorReduced(M);

  // Row vectot representing the current shape overlaid onto the sample
  app->setGraphic(sample);
  Mat sampleRow = app->toRowVectorReduced(M);

  return Aux::mse(sampleRow, selfRow);
}

void FittedAAM::drawOverlay(Mat& canvas)
{
}
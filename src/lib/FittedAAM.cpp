#include "FittedAAM.h"

BaseFittedModel* FittedAAM::setOrigin(const Point2d& p)
{
  this->origin = p;
  return this;
}

const double FittedAAM::getMeanShapeScale()
{
  return this->pcaAppearance().getScale();
}

BaseFittedModel* FittedAAM::setShapeParam(const Mat& param)
{
  param.copyTo(this->shapeParam);
  return this;
}

BaseFittedModel* FittedAAM::setAppearanceParam(const Mat& param)
{
  param.copyTo(this->appearanceParam);
  return this;
}

Appearance* FittedAAM::toAppearance()
{
  assert(this->scale > 0);
  assert(this->origin.x >= 0);
  assert(this->origin.y >= 0);

  // Override scale and translation
  auto app = this->pcaAppearance().toAppearance(appearanceParam);
  Point2d origin0 = this->pcaAppearance().getBound().tl();
  Point2d displacement = origin0 - this->origin;
  app->recentre(displacement);
  
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
  auto cloned = new FittedAAM(this->aamPCA);
  cloned->setOrigin(this->origin);
  cloned->setShapeParam(this->shapeParam);
  cloned->setAppearanceParam(this->appearanceParam);
  return cloned;
}
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

  // TAOTODO: check if works
  return this->pcaAppearance()
    .cloneWithNewScale(scale, origin)
    .toAppearance(appearanceParam);
}

MeshShape* FittedAAM::toShape()
{
  return this->pcaShape().toShape(this->shapeParam);
}

Rect FittedAAM::getBound() const
{
  // Offset and scaled bound of the shape
  auto b = aamPCA->getBound();
  b.width *= this->scale;
  b.height *= this->scale;
  b.x += this->origin.x;
  b.y += this->origin.y;
  return b;
}

double FittedAAM::measureError(const Mat& sample)
{
  // TAOTODO: New approach:
  // - Draw the model as overlay on black canvas
  // - Offset and rescale the overlay
  // - Crop the sample by shape boundary
  // - Measure error

  Rect bound = getBound();
  Mat canvas = Mat::zeros(Size(bound.x + bound.width + 1, bound.y + bound.height + 1), CV_8UC3);
  cout << "bound of AAM ~ " << bound << endl; // TAODEBUG:

  drawOverlay(canvas);

  // TAOTODO: Draw contour of appearance as boundary of computation


  return 0;
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
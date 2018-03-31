#include "FittedAAM.h"

BaseFittedModel* FittedAAM::setOrigin(const Point2d& p)
{
  this->origin = p;
  return this;
}

const double FittedAAM::getMeanShapeScale() const
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

Appearance* FittedAAM::toAppearance() const
{
  assert(this->scale > 0);
  assert(this->origin.x >= 0);
  assert(this->origin.y >= 0);

  // TAOTODO: check if works
  return this->pcaAppearance()
    .cloneWithNewScale(scale, origin)
    .toAppearance(appearanceParam);
}

MeshShape* FittedAAM::toShape() const
{
  // With scaling and translation
  auto shape = this->pcaShape().toShape(this->shapeParam);
  shape->recentreAndScale(origin, scale);
  auto shape_ = new MeshShape(*shape);
  return shape_;
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

  // Draw contour of appearance as boundary of computation
  auto shape = toShape();
  Mat shapeConvex = shape->convexFill();

  // TAODEBUG:
  destroyAllWindows();
  imshow("overlay", canvas);
  imshow("convex", shapeConvex);
  waitKey(0);

  return 0;
}

void FittedAAM::drawOverlay(Mat& canvas)
{
  IO::MatIO m;
  Appearance* app = this->toAppearance();

  // TAODEBUG: check appearance
  cout << RED << "[appearance]" << RESET << endl;
  cout << app->getGraphic().size() << endl;
  cout << "bound : " << app->getSpannedSize() << endl;
  imshow("graphic", app->getGraphic());
  waitKey(0);

  app->render(&m, canvas);
  // TAOTODO: Canvas is wrongly painted, it's entirely blackout
}

BaseFittedModel* FittedAAM::clone() const
{
  auto cloned = new FittedAAM(this->aamPCA);
  cloned->setOrigin(this->origin);
  cloned->setShapeParam(this->shapeParam);
  cloned->setAppearanceParam(this->appearanceParam);
  return cloned;
}
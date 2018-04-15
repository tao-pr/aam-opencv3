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
  // - Draw the model as overlay on black canvas
  // - Offset and rescale the overlay
  // - Crop the sample by shape boundary
  // - Measure aggregated error of intensity

  Rect bound = getBound();
  Mat canvas = Mat::zeros(Size(bound.x + bound.width + 1, bound.y + bound.height + 1), CV_8UC3);
  Mat overlay = drawOverlay(canvas);

  // Draw contour of appearance as boundary of computation
  auto shape = toShape();
  Mat shapeConvex = shape->convexFill();

  // RMSE
  double e = 0;
  int n = 0;
  for (int i=0; i<canvas.cols; i++)
    for (int j=0; j<canvas.rows; j++)
    {
      if (shapeConvex.at<unsigned char>(j,i) > 0 &&
          sample.cols > i && 
          sample.rows > j)
      {
        ++n;
        auto a = overlay.at<Vec3b>(j,i);
        auto b = sample.at<Vec3b>(j,i);
        auto d = a - b;
        e += (Aux::square(d[0]) + Aux::square(d[1]) + Aux::square(d[2])) * 0.33;
      }
    }

  // Error per pixel
  return Aux::sqrt(e) / (double)n;
}

Mat FittedAAM::drawOverlay(Mat& canvas, bool withEdges)
{
  IO::MatIO m;
  Appearance* app = this->toAppearance();
  
  Mat gr = app->getGraphic();
  auto size = app->getShape().getBound();
  rectangle(gr, size.tl(), size.br(), Scalar(0,100,200), 1);

  app->render(&m, canvas, withEdges, withEdges);
  return m.get();
}

unique_ptr<BaseFittedModel> FittedAAM::clone() const
{
  unique_ptr<BaseFittedModel> cloned{ new FittedAAM(this->aamPCA) };
  cloned->setOrigin(this->origin);
  cloned->setShapeParam(this->shapeParam);
  cloned->setAppearanceParam(this->appearanceParam);
  return cloned;
}
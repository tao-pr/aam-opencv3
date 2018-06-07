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

  auto shape = toShape();
  auto appearance = this->pcaAppearance()
    .cloneWithNewScale(scale, origin)
    .toAppearance(appearanceParam);

  // TAODEBUG:
  // cout << "old vertices : "
  //       << appearance->getShape().getMat() << endl;
  // cout << "new vertices : "
  //       << shape->getMat() << endl; 

  appearance->realignTo(*shape);
  return appearance;
}

MeshShape* FittedAAM::toShape() const
{
  // TAOTOREVIEW: Following has an issue if the shape param is big (high variation).
  // After a conversion from PCA to shape,
  // the outcome may generate a different MeshShape triangle sequence.
  // This has to be synchronised with the original (mean) shape.

  // TAOTOREVIEW: Generate the shape from PCA

  // TAOTOREVIEW: Map new vertices with their closest the mean shape vertices (reorder if necessary)

  // TAOTOREVIEW: Update SubDiv2D with the new vertices positions

  // That's it!

  auto shape = this->pcaShape().toShape(this->shapeParam);
  auto shape_ = shape->recentreAndScale(origin, scale);

  // TAODEBUG:
  // cout << "PCA->toShape" << endl;
  // cout << "before scaling : " << shape->getMat() << endl;
  // cout << "after scaling : " << shape_.getMat() << endl;
  // cout << "PCA->toShape [END]" << endl;

  return new MeshShape(shape_);
}

Size FittedAAM::getSpannedSize() const
{
  Rect bound = getBound();
  return Size(bound.x + bound.width, bound.y + bound.height);
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
  auto shape = toShape();
  Mat shapeConvexOriginal = shape->convexFill();

  // Find the biggest possible rectangle which is capable of containing the following:
  // - The convex fill of the shape
  // - The sample boundary
  // - The model boundary
  int minX = max(0, min(bound.x, sample.cols-1));
  int minY = max(0, min(bound.y, sample.rows-1));
  int maxX = min(sample.cols-1, bound.x+bound.width);
  int maxY = min(sample.rows-1, bound.y+bound.height);
  maxX = min(shapeConvexOriginal.cols-1, maxX);
  maxY = min(shapeConvexOriginal.rows-1, maxY);
  Rect obound(minX, minY, maxX-minX, maxY-minY);  

  Mat canvas = Mat::zeros(Size(bound.x + bound.width + 1, bound.y + bound.height + 1), CV_8UC3);
  Mat overlay = drawOverlay(canvas)(obound);
  Mat sampleCrop = sample(obound);

  Mat shapeConvex = shapeConvexOriginal(obound);
  Mat shapeConvexBGR(obound.height, obound.width, CV_8UC3);
  cvtColor(shapeConvex, shapeConvexBGR, CV_GRAY2BGR);

  Mat diff(obound.height, obound.width, CV_8UC3);

  // Geometrically crop the diff between the sample and the overlay
  absdiff(overlay, sampleCrop, diff);
  bitwise_and(diff, shapeConvexBGR, diff);

  // Find RMSE error
  double e = 0;
  double n = 0;
  for (int i=0; i<obound.width; i++)
    for (int j=0; j<obound.height; j++)
    {
      if (shapeConvex.at<unsigned char>(j,i) > 0)
      {
        auto d = diff.at<Vec3b>(j,i);
        e += Aux::square(0.33*(d[0] + d[1] + d[2]));
        n += 1;
      }
    }
  e = (n == 0) ? numeric_limits<double>::max() : Aux::sqrt(e/n);

  // TAODEBUG:
  // destroyAllWindows();
  // imshow("overlay", overlay);
  // imshow("sample", sample);
  // imshow("diff", diff);

  return e;
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
  cloned->scale = this->scale;
  return cloned;
}
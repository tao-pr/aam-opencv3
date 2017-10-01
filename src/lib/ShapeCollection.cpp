#include "ShapeCollection.h"

ShapeCollection ShapeCollection::clone(bool isVerbose) const
{
  return ShapeCollection(this->items, isVerbose);
}

ShapeCollection ShapeCollection::normaliseScalingTranslation() const
{
  // Rescale each shape so the centroid size = 1
  // and translate to the centroid
  vector<Shape> scaled;
  for (auto shape : this->items)
  {
    auto centroid = shape.centroid();
    auto cdist    = shape.sumSquareDistanceToPoint(centroid);
    scaled.push_back((shape << centroid) * (1.0/cdist));
  }
  return ShapeCollection(scaled, verbose);
}

ShapeCollection ShapeCollection::translateBy(const Point2d &p) const
{
  vector<Shape> tr;
  for (auto shape : this->items)
  {
    tr.push_back(shape >> p);
  }
  return ShapeCollection(tr, verbose);
}

ShapeCollection ShapeCollection::normaliseRotation() const
{
  vector<Shape> norml;
  // Use the first shape as base rotation = 0
  Mat x0 = this->items[0].mat;
  for (auto shapeIter=this->items.begin()+1; shapeIter!=this->items.end(); shapeIter++)
  {
    // Compute SVD on xj•x0
    Mat xj = shapeIter->mat;
    Mat xx = xj.t() * x0;
    auto svd = SVD(xx, SVD::FULL_UV);
    auto u = svd.u;
    auto v = svd.vt.t();
    Mat R = v * u.t(); // TAOTODO: How to convert <MatExpr> to <Mat> ?

    if (verbose)
    {
      double angle = acos(R.at<double>(0,0)) / M_PI;
      cout << "... Compensating rotation of next shape (angle = " << angle << "π)" << endl;
    }

    // rotate shape by theta and store in norm<>
    norml.push_back(Shape((R * xj.t()).t()));
  }

  return ShapeCollection(norml, verbose);
}

vector<Shape> ShapeCollection::getItems() const
{
  return this->items;
}

double ShapeCollection::sumProcrustesDistance(const Shape& targetShape) const
{
  double sumDist = 0.0;
  for (auto shape : this->items)
  {
    sumDist += shape.procrustesDistance(targetShape);
  }
  return sumDist;
}

/**
 * Compute the Procrustes mean shape and the aligned collection onto that mean
 */
tuple<Shape, ShapeCollection> ShapeCollection::procrustesMeanShape(double tol, int maxIter) const
{
  // Pick the first shape from the collection as initial mean
  auto mean        = this->items[0];
  auto alignedSet  = this->clone();
  double err       = numeric_limits<double>::max();
  double lastError = 0;
  int iter         = 0;

  auto tolerance = [&](double e, double e0)
  {
    return abs(e-e0)/min(e0,e);
  };

  if (verbose) cout << GREEN << "[Procrustes Mean shape]" << RESET << endl;
  while (tolerance(err, lastError) > tol && iter < maxIter)
  {
    if (verbose) cout << CYAN << "... Aligning iter# " << RESET << iter << endl;
    alignedSet = alignedSet.normaliseRotation();
    err = alignedSet.sumProcrustesDistance(mean);
    if (verbose) cout << "... Error so far : " << err << endl;
    if (verbose) cout << "... tol : " << tolerance(err, lastError) << endl;

    maxIter++;
    lastError = err;
  }

  return make_tuple(mean, alignedSet);
}

void ShapeCollection::renderShapeVariation(IO::GenericIO* io, Size sz, double scaleFactor, Point2d recentred) const
{
  Mat canvas = Mat::zeros(sz, CV_8UC3);
  for (auto shape : this->items)
  {
    canvas = shape.render(io, canvas, scaleFactor, recentred);
    waitKey(100);
  }
}
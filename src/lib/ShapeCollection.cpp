#include "ShapeCollection.h"

ShapeCollection::ShapeCollection(const vector<Shape*>& shapes, const bool isVerbose)
{
  this->verbode = isVerbose;
  vector<BaseModel*> vs;
  for (auto shape : shapes)
  {
    vs.push_back(shape);
  }
  this->items = vs;
}

ShapeCollection::ShapeCollection(const ShapeCollection& original)
{
  ShapeCollection(original.items, original.verbose);
}

unique_ptr<ModelCollection> ShapeCollection::clone(bool isVerbose) const
{
  // Deep copy
  vector<BaseModel*> clonedVector;
  for (auto model : this->items)
  {
    clonedVector.push_back(new Shape(*model));
  }
  return {new ShapeCollection(clonedVector, isVerbose)};
}

unique_ptr<ModelCollection> ShapeCollection::normaliseScalingTranslation() const
{
  // Rescale each shape so the centroid size = 1
  // and translate to the centroid
  vector<Shape*> scaled;
  for (auto model : this->items)
  {
    auto shape = dynamic_cast<Shape*>(model);
    auto centroid = shape->centroid();
    auto cdist    = shape->sumSquareDistanceToPoint(centroid);
    scaled.push_back(new Shape((*shape << centroid) * (1.0/cdist)));
  }
  return {new ShapeCollection(scaled, verbose)};
}

unique_ptr<ModelCollection> ShapeCollection::translateBy(const Point2d &p) const
{
  vector<Shape*> tr;
  for (auto model : this->items)
  {
    Shape* shape = dynamic_cast<Shape*>(model);
    tr.push_back(new Shape(shape >> p));
  }
  return {new ShapeCollection(tr, verbose)};
}

unique_ptr<ModelCollection> ShapeCollection::normaliseRotation() const
{
  // Use the first shape as base rotation = 0
  Mat x0 = this->items[0]->getMat();
  vector<Shape*> norml{ new Shape(x0); };
  
  for (auto shapeIter=this->items.begin()+1; shapeIter!=this->items.end(); shapeIter++)
  {
    // Compute SVD on xj•x0
    Mat xj = shapeIter->getMat();
    Mat xx = xj.t() * x0;
    auto svd = SVD(xx, SVD::FULL_UV);
    auto u = svd.u;
    auto v = svd.vt.t();
    Mat R = v * u.t();

    if (verbose)
    {
      double angle = acos(R.at<double>(0,0)) / M_PI;
      cout << "... Compensating rotation of next shape (angle = " << angle << "π)" << endl;
    }

    // rotate shape by theta and store in norm<>
    norml.push_back(new Shape((R * xj.t()).t()));
  }

  return {new ShapeCollection(norml, verbose)};
}

vector<Shape*> ShapeCollection::getShapeItems() const
{
  return this->items;
}

/**
 * Convert the collection to a matrix
 * where each row represents a distinct shape
 */
Mat ShapeCollection::toMat() const
{
  int M = this->items[0]->toMat().rows;
  int N = this->items.size();
  Mat m = Mat(N, M*2, CV_64FC1);
  int j = 0;
  for (auto shape : this->items)
  {
    shape.toRowVector().row(0).copyTo(m.row(j));
    j++;
  }
  return m;
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
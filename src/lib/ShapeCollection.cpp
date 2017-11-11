#include "ShapeCollection.h"

ShapeCollection::ShapeCollection(const vector<Shape*> shapes, const bool isVerbose)
{
  this->verbode = isVerbose;
  vector<BaseModel*> vs;
  for (auto shape : shapes)
  {
    vs.push_back(shape);
  }
  this->items = vs;
}

ModelCollection ShapeCollection::clone(bool isVerbose) const
{
  // Deep copy
  vector<BaseModel*> clonedVector;
  for (auto model : this->items)
  {
    clonedVector.push_back(new Shape(*model));
  }
  return ShapeCollection(clonedVector, isVerbose);
}

ShapeCollection ShapeCollection::normaliseScalingTranslation() const
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
  return ShapeCollection(scaled, verbose);
}

ShapeCollection ShapeCollection::translateBy(const Point2d &p) const
{
  vector<Shape*> tr;
  for (auto model : this->items)
  {
    Shape* shape = dynamic_cast<Shape*>(model);
    tr.push_back(new Shape(shape >> p));
  }
  return ShapeCollection(tr, verbose);
}

ShapeCollection ShapeCollection::normaliseRotation() const
{
  // Use the first shape as base rotation = 0
  Mat x0 = this->items[0].getMat();
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

  return ShapeCollection(norml, verbose);
}

vector<Shape*> ShapeCollection::getShapeItems() const
{
  return this->items;
}

double ShapeCollection::sumProcrustesDistance(const Shape* targetShape) const
{
  double sumDist = 0.0;
  for (auto model : this->items)
  {
    sumDist += model->procrustesDistance(targetShape);
  }
  return sumDist;
}

Mat ShapeCollection::covariance(const Shape& mean) const
{
  int shapeSize = this->items[0].mat.rows;
  Mat cov = Mat::zeros(shapeSize, shapeSize, CV_64FC1);
  double N = 0;
  for (auto shape : this->items)
  {
    auto res = shape.mat - mean.mat;
    cov = cov + res * res.t();
    N += 1.0;
  }
  return cov * (1/N);
}

/**
 * Convert the collection to a matrix
 * where each row represents a distinct shape
 */
Mat ShapeCollection::toMat() const
{
  int M = this->items[0].mat.rows;
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

/**
 * Compute eigenvectors of the covariance matrix of the shapes
 */
ModelEncoder ShapeCollection::pca(const Shape& meanShape) const
{
  if (verbose) cout << GREEN << "[Computing Shape PCA]" << RESET << endl;
  Mat meanVector = meanShape.toRowVector();
  Mat data       = this->toMat();
  if (verbose) 
  {
    cout << "... mean shape size : " << meanVector.rows << " x " << meanVector.cols << endl;
    cout << "... data size       : " << data.rows << " x " << data.cols << endl;
  }
  auto pca = PCA(data, meanVector, CV_PCA_DATA_AS_ROW);

  // Collect lambdas
  // TAOTOREVIEW: Take only highest K lambda where K<N
  int N = pca.eigenvalues.rows;
  if (verbose)
  {
    cout << "... eigenvalues  : " << N << endl;
    cout << "... eigenvectors : " << pca.eigenvectors.rows << " x " << pca.eigenvectors.cols << endl;
  }

  // Compose a shape param set from eigenvalues
  return ModelEncoder(meanShape.toColVector(), pca.eigenvectors);
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
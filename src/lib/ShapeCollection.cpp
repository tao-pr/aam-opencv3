#include "ShapeCollection.h"

ShapeCollection::ShapeCollection(const vector<Shape*>& shapes)
: ModelCollection()
{
  vector<BaseModel*> vs;
  for (auto shape : shapes)
  {
    vs.push_back(shape);
  }
  this->items = vs;
}

unique_ptr<ModelCollection> ShapeCollection::clone() const
{
  #ifdef DEBUG
  cout << "BaseModelShapeCollection::clone " << getUID() << endl;
  #endif
  // Deep copy
  vector<BaseModel*> clonedVector;
  for (auto model : this->items)
  {
    auto shape = dynamic_cast<Shape*>(model);
    clonedVector.push_back(new Shape(*shape));
  }
  unique_ptr<ModelCollection> newSet(new ShapeCollection(clonedVector));
  return newSet;
}

void ShapeCollection::normaliseScalingTranslation()
{
  #ifdef DEBUG
  cout << "BaseModelShapeCollection::normaliseScalingTranslation @" << getUID() << endl;
  #endif
  // Rescale each shape so the centroid size = 1
  // and translate to the centroid
  vector<BaseModel*> scaled;
  for (auto model : this->items)
  {
    auto shape = dynamic_cast<Shape*>(model);
    auto centroid = shape->centroid();
    auto cdist    = shape->sumSquareDistanceToPoint(centroid);
    scaled.push_back(new Shape((*shape << centroid) * (1.0/cdist)));
  }
  // Replace with new items
  clear();
  swap(this->items, scaled);
}

void ShapeCollection::translateBy(const Point2d &p)
{
  vector<BaseModel*> tr;
  for (auto model : this->items)
  {
    Shape* shape = dynamic_cast<Shape*>(model);
    tr.push_back(new Shape(*shape >> p));
  }
  clear();
  swap(this->items, tr);
}

void ShapeCollection::normaliseRotation()
{
  #ifdef DEBUG
  cout << "ShapeCollection::normaliseRotation" << endl;
  #endif

  // Use the first shape as base rotation = 0
  Mat x0 = this->items[0]->getMat();
  vector<BaseModel*> norml{ new Shape(x0) };
  
  for (auto shapeIter=this->items.begin()+1; shapeIter!=this->items.end(); shapeIter++)
  {
    // Compute SVD on xj•x0
    Mat xj = (*shapeIter)->getMat();
    Mat xx = xj.t() * x0;
    auto svd = SVD(xx, SVD::FULL_UV);
    auto u = svd.u;
    auto v = svd.vt.t();
    Mat R = v * u.t();

    #ifdef DEBUG
    double angle = acos(R.at<double>(0,0)) / M_PI;
    cout << "... Compensating rotation of next shape (angle = " << angle << "π)" << endl;
    #endif

    // rotate shape by theta and store in norm<>
    norml.push_back(new Shape((R * xj.t()).t()));
  }

  clear();
  swap(this->items, norml);
}

/**
 * Convert the collection to a matrix
 * where each row represents a distinct shape
 */
Mat ShapeCollection::toMat() const
{
  int N = this->items.size();
  int M = this->items[0]->toRowVector().cols;
  Mat m = Mat(N, M, CV_64FC1);

  // TAODEBUG:
  auto v  =this->items[0]->toRowVector();
  cout << "v = " << v.rows << " x " << v.cols << endl;

  int j = 0;
  for (auto model : this->items)
  {
    model->toRowVector().row(0).copyTo(m.row(j));
    j++;
  }
  return m;
}

void ShapeCollection::renderShapeVariation(IO::GenericIO* io, Size sz, double scaleFactor, Point2d recentred) const
{
  Mat canvas = Mat::zeros(sz, CV_8UC3);
  for (auto model : this->items)
  {
    auto shape = dynamic_cast<Shape*>(model);
    canvas = shape->render(io, canvas, scaleFactor, recentred);
    waitKey(100);
  }
}
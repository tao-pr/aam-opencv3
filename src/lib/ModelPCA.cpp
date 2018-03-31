#include "ModelPCA.h"

Mat ModelPCA::toParam(const BaseModel* m) const
{
  Mat vec = m->toRowVector();
  return this->pca.project(vec);
}

BaseModel* ShapeModelPCA::mean() const
{
  return new MeshShape(this->pca.mean);
}

BaseModel* ShapeModelPCA::toModel(const Mat& param) const 
{
  return toShape(param);
}

MeshShape* ShapeModelPCA::toShape(const Mat& param) const
{
  Mat shapeParam = this->pca.backProject(param).reshape(1, param.cols/2);
  return new MeshShape(shapeParam);
}

ShapeModelPCA ShapeModelPCA::cloneWithNewScale(double newScale, const Point2d& newTranslation) const
{
  ShapeModelPCA neue(*this);
  neue.setScale(newScale);
  neue.setTranslation(newTranslation);
  return neue;
}

Mat* ShapeModelPCA::permutationOfParams() const
{
  double step = 0.01;
  auto perm = new Mat[dimension()*2];
  for (int i=0; i<dimension()*2; i++)
  {
    if (i%2 == 0)
    {
      perm[i] = Mat::zeros(0, dimension(), CV_64FC1);
      perm[i].at<double>(0, i) = step;
    }
    else
    {
      perm[i] = Mat::zeros(0, dimension(), CV_64FC1);
      perm[i].at<double>(0, i) = -step;
    }
  }
  return perm;
}

BaseModel* AppearanceModelPCA::mean() const
{
  // TAOTODO: Should also shift and scale
  auto bound = meanShape.getBound();
  auto N = bound.width * bound.height;
  auto K = pca.mean.cols/3;

  // Reshape the row vector into a spatial graphic for the appearance
  Mat graphic = Mat(bound.height + bound.y, bound.width + bound.x, CV_8UC3, Scalar(0,0,0)); 
  
  // Split mean vector into 3 channels, scale them to the expected size
  vector<Mat> meanChannels;
  Mat meanGraphic;
  for (int i=0; i<3; i++)
  {
    Mat m = this->pca.mean(Rect(i*K, 0, K, 1)).clone();
    Mat c = Mat(1, N, CV_64FC1);
    resize(m, c, Size(N, 1));
    Mat meanCh = Mat(Size(N, 1), CV_8UC1);
    c.convertTo(meanCh, CV_8UC1);
    meanChannels.push_back(meanCh.reshape(1, bound.height));
  }
  merge(meanChannels, meanGraphic);
  meanGraphic.copyTo(graphic(Rect(bound)));

  return new Appearance(meanShape, graphic);
}

BaseModel* AppearanceModelPCA::toModel(const Mat& param) const
{
  return toAppearance(param);
}

Mat AppearanceModelPCA::toParam(const BaseModel* m) const
{
  const Appearance* app = dynamic_cast<const Appearance*>(m);
  Mat vec = app->toRowVectorReduced(this->pca.mean.cols);
  return this->pca.project(vec);
}

void AppearanceModelPCA::overrideMeanShape(const MeshShape& newMeanShape)
{
  this->meanShape = newMeanShape;
}

Mat* AppearanceModelPCA::permutationOfParams() const
{
  double step = 0.001;
  auto perm = new Mat[dimension()*2];
  for (int i=0; i<dimension()*2; i++)
  {
    if (i%2 == 0)
    {
      perm[i] = Mat::zeros(0, dimension(), CV_64FC1);
      perm[i].at<double>(0, i) = step;
    }
    else
    {
      perm[i] = Mat::zeros(0, dimension(), CV_64FC1);
      perm[i].at<double>(0, i) = -step;
    }
  }
  return perm;
}

Rect AppearanceModelPCA::getBound() const
{
  Rect b = meanShape.getBound();
  b.width *= this->scale;
  b.height *= this->scale;
  b.x += this->translation.x;
  b.y += this->translation.y;
  return b;
}

AppearanceModelPCA AppearanceModelPCA::cloneWithNewScale(double newScale, const Point2d& newTranslation) const
{
  // TAOTODO: Should also rescale or translate meanshape
  AppearanceModelPCA neue(*this);
  neue.setScale(newScale);
  neue.setTranslation(newTranslation);
  return neue;
}

Appearance* AppearanceModelPCA::toAppearance(const Mat& param) const
{
  auto meanShapeOffset = MeshShape(this->meanShape.recentreAndScale(translation, scale));
  auto bound = meanShape.getBound();
  auto offsetBound = this->getBound();
  auto N = bound.width * bound.height;
  auto K = pca.mean.cols/3;

  #ifdef DEBUG
  cout << "Converting PCA -> Appearance Model" << endl;
  cout << "-> bound        : " << bound << endl;
  cout << "-> offset bound : " << offsetBound << endl;
  cout << "-> pca mean     : " << this->pca.mean.size() << endl;
  cout << "-> eigenvectors : " << this->pca.eigenvectors.size() << endl;
  cout << "-> params       : " << param.size() << endl;
  #endif

  // Backprojection of appearance params
  Mat backPrj = this->pca.backProject(param);

  // Reshape the row vector into a spatial graphic for the appearance
  Mat graphic = Mat(
    offsetBound.height + offsetBound.y, 
    offsetBound.width + offsetBound.x, 
    CV_8UC3, Scalar(0,0,0));
  
  // Split backprojected vector into 3 channels, scale them to the expected size
  vector<Mat> bpjChannels;
  Mat bpjGraphic;
  for (int i=0; i<3; i++)
  {
    Mat m = backPrj(Rect(i*K, 0, K, 1)).clone();
    Mat c = Mat(1, N, CV_64FC1);
    resize(m, c, Size(N, 1));
    Mat meanCh = Mat(Size(N, 1), CV_8UC1);
    c.convertTo(meanCh, CV_8UC1);
    bpjChannels.push_back(meanCh.reshape(1, bound.height));
  }
  merge(bpjChannels, bpjGraphic);
  bpjGraphic.copyTo(graphic(Rect(bound)));

  if (scale == 1 && translation == Point2d(0,0))
    return new Appearance(meanShapeOffset, graphic);
  else
  {
    #ifdef DEBUG
    cout << "-> scaling : " << scale << endl;
    cout << "-> translation : " << translation << endl;
    #endif

    MeshShape meanOffsetShape(meanShape.recentreAndScale(translation, scale));  
    // TAODEBUG:
    auto ioMesh = IO::WindowIO("meanShapeOffset");
    meanOffsetShape.render(&ioMesh, Mat::zeros(graphic.size(), CV_8UC3));
    waitKey(0);

    return new Appearance(meanOffsetShape, graphic);
  }
}



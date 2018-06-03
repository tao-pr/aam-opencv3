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

int ShapeModelPCA::getSizeOfPermutationOfParams() const
{
  int K = dimension();
  int M = 12 * K;
  return M;
}

void ShapeModelPCA::permutationOfParams(Mat* out) const
{
  double steps[] = {0.01, -0.01, 0.1, -0.1, 1, -1};
  int K = dimension();
  int M = 12 * K;
  int n = 0;
  for (auto step : steps)
  {
    for (int i=0; i<K*2; i++)
    {
      if (i%2 == 0)
      {
        out[n] = Mat::zeros(1, K, CV_64FC1);
        out[n].at<double>(0, i/2) = step;
      }
      else
      {
        out[n] = Mat::zeros(1, K, CV_64FC1);
        out[n].at<double>(0, (i-1)/2) = -step;
      }
      ++n;
    }
  }
  assert(M == n);
}

BaseModel* AppearanceModelPCA::mean() const
{
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

int AppearanceModelPCA::getSizeOfPermutationOfParams() const
{
  int K = dimension();
  int M = 6 * K * 2;
  return M;
}

void AppearanceModelPCA::permutationOfParams(Mat* out) const
{
  double steps[] = {0.1, -0.1, 0.01, -0.01, 1, -1};
  int K = dimension();
  int M = 6 * K * 2;
  int n = 0;
  for (auto step : steps)
  {
    for (int i=0; i<K*2; i++)
    {
      if (i%2 == 0)
      {
        out[n] = Mat::zeros(1, K, CV_64FC1);
        out[n].at<double>(0, i/2) = step;
      }
      else
      {
        out[n] = Mat::zeros(1, K, CV_64FC1);
        out[n].at<double>(0, (i-1)/2) = -step;
      }
      ++n;
    }
  }
  assert(M == n);
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
  AppearanceModelPCA neue(*this);
  neue.setTranslation(newTranslation);
  neue.setScale(newScale);
  return neue;
}

Appearance* AppearanceModelPCA::toAppearance(const Mat& param) const
{
  // Generate a mean appearance model
  // then apply translation, scaling, and parameters later

  auto bound = meanShape.getBound();
  auto N = bound.width * bound.height;
  auto K = pca.mean.cols/3;

  Mat modelInitGraphic;

  // Backprojection from PCA parameters to image
  Mat backPrj = this->pca.backProject(param);
  
  // Split backprojected vector into 3 channels, scale them to the expected size
  vector<Mat> bpjChannels;

  for (int i=0; i<3; i++)
  {
    Mat m = backPrj(Rect(i*K, 0, K, 1)).clone();
    Mat c = Mat(1, N, CV_64FC1);
    resize(m, c, Size(N, 1));
    Mat meanCh = Mat(Size(N, 1), CV_8UC1);
    c.convertTo(meanCh, CV_8UC1);
    bpjChannels.push_back(meanCh.reshape(1, bound.height));
  }
  merge(bpjChannels, modelInitGraphic);

  // TAODEBUG:
  cout << "nonZeros [0] = " << countNonZero(bpjChannels[0]) << endl;
  cout << "nonZeros [1] = " << countNonZero(bpjChannels[1]) << endl;
  cout << "nonZeros [2] = " << countNonZero(bpjChannels[2]) << endl;

  // Rescale and re-position the graphic
  int tx = translation.x;
  int ty = translation.y;
  int w0 = (int)ceil(modelInitGraphic.cols*scale);
  int h0 = (int)ceil(modelInitGraphic.rows*scale);
  int w = w0 + tx;
  int h = h0 + ty;
  Mat modelGraphic = Mat::zeros(h, w, CV_8UC3);
  resize( 
    modelInitGraphic,
    modelGraphic(Rect(tx, ty, w0, h0)),
    Size(w0, h0));

  // TAODEBUG:
  cout << "bound : " << bound << endl;
  cout << param << endl;
  imshow("init graphic", modelInitGraphic);
  imshow("resized graphic", modelGraphic);
  waitKey(0);

  // Rescale and re-position the shape
  cout << "Rescaling shape ..." << endl; // TAODEBUG:
  auto modelShape = MeshShape(meanShape);
  modelShape.recentreAndScale(translation, scale);

  // Create an appearance out of the rescaled and translated shapes & graphic
  cout << "creating appearance model..." << endl; // TAODEBUG:
  auto appearance = new Appearance(modelShape, modelGraphic);
  return appearance;

  // auto originalOffset = meanShape.getBound();
  // auto meanShapeOffset = MeshShape(this->meanShape.recentreAndScale(translation, scale));
  // auto bound = meanShape.getBound();
  // auto newOffset = meanShapeOffset.getBound();
  // auto N = bound.width * bound.height;
  // auto K = pca.mean.cols/3;

  // // Backprojection of appearance params
  // Mat backPrj = this->pca.backProject(param);
  
  // // Split backprojected vector into 3 channels, scale them to the expected size
  // vector<Mat> bpjChannels;
  // Mat bpjGraphic;
  // for (int i=0; i<3; i++)
  // {
  //   Mat m = backPrj(Rect(i*K, 0, K, 1)).clone();
  //   Mat c = Mat(1, N, CV_64FC1);
  //   resize(m, c, Size(N, 1));
  //   Mat meanCh = Mat(Size(N, 1), CV_8UC1);
  //   c.convertTo(meanCh, CV_8UC1);
  //   bpjChannels.push_back(meanCh.reshape(1, bound.height));
  // }
  // merge(bpjChannels, bpjGraphic);
  // int w_ = (int)ceil(bpjGraphic.cols*scale);
  // int h_ = (int)ceil(bpjGraphic.rows*scale);
  // Rect offsetBound(
  //   translation.x + originalOffset.x,
  //   translation.y + originalOffset.y,
  //   w_,
  //   h_);
  // Mat graphic(Mat::zeros(
  //   h_ + translation.y + originalOffset.y, 
  //   w_ + translation.x + originalOffset.x, 
  //   CV_8UC3));
  // if (scale != 1)
  // {
  //   Mat tmp(h_, w_, CV_8UC3);
  //   resize(bpjGraphic, tmp, Size(w_, h_));
  //   tmp.copyTo(graphic(offsetBound));

  //   // TAODEBUG:
  //   rectangle(graphic, originalOffset.tl(), originalOffset.br(), Scalar(200,0,0), 1);
  //   rectangle(graphic, newOffset.tl(), newOffset.br(), Scalar(0,150,250), 3);
  // }
  // else
  // {
  //   bpjGraphic.copyTo(graphic(Rect(offsetBound)));
  // }

  // if (scale == 1 && translation == Point2d(0,0))
  //   return new Appearance(meanShapeOffset, graphic);
  // else
  // {
  //   // MeshShape meanOffsetShape(meanShape.recentreAndScale(translation, scale));  
  //   return new Appearance(meanShapeOffset, graphic);
  // }
}



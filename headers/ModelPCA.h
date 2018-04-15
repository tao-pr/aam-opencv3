#ifndef MODEL_PCA
#define MODEL_PCA

#include "master.h"
#include "BaseModel.h"
#include "MeshShape.h"
#include "Appearance.h"

/**
 * PCA model encoding
 * All models here assume the data is stored as row vector.
 */
class ModelPCA 
{
protected:
  PCA pca;

  // Overriding parameters
  Point2d translation;
  double scale;

public:
  ModelPCA() : translation(Point2d(0,0)), scale(1) {};
  ModelPCA(const PCA& p) : pca(p), translation(Point2d(0,0)), scale(1) {};
  ModelPCA(const PCA& p, const Point2d &t, const double s) : pca(p), translation(t), scale(s) {};
  inline virtual ~ModelPCA() {};

  // Encode a model to a parameter set (row vector)
  virtual Mat toParam(const BaseModel* m) const;
  virtual BaseModel* mean() const = 0;
  virtual BaseModel* toModel(const Mat& param) const = 0;

  // Generate permutation of parameter matrix
  virtual vector<Mat> permutationOfParams() const = 0;

  const int dimension() const { return this->pca.eigenvalues.rows; };
  Point2d getTranslation() const { return this->translation; };
  double getScale() const { return this->scale; };
  void setTranslation(const Point2d& t) { this->translation = t; };
  void setScale(const double& s) { this->scale = s; };
};

class ShapeModelPCA : public ModelPCA 
{ 
public: 
  ShapeModelPCA() : ModelPCA() {};
  ShapeModelPCA(const PCA& p) : ModelPCA(p) {};
  ShapeModelPCA(const PCA& p, const Point2d& t, const double s) : ModelPCA(p) {};
  BaseModel* mean() const; 
  BaseModel* toModel(const Mat& param) const;
  MeshShape* toShape(const Mat& param) const;
  vector<Mat> permutationOfParams() const;

  ShapeModelPCA cloneWithNewScale(double newScale, const Point2d& newTranslation) const;
};

class AppearanceModelPCA : public ModelPCA 
{ 
protected:
  Size originalBound;
  MeshShape meanShape; // TAOTODO: Should enfore meanShape origin at (0,0)
public: 
  AppearanceModelPCA() : ModelPCA() {};
  AppearanceModelPCA(const PCA& p, const MeshShape& mean, const Size& size) : ModelPCA(p), originalBound(size) { this->meanShape = mean; };
  AppearanceModelPCA(const AppearanceModelPCA& that) : ModelPCA(that.pca) 
  { 
    originalBound = that.originalBound;
    meanShape = that.meanShape;
  };
  BaseModel* mean() const;
  
  AppearanceModelPCA cloneWithNewScale(double newScale, const Point2d& newTranslation) const;
  Mat toParam(const BaseModel* m) const;
  BaseModel* toModel(const Mat& param) const;
  Appearance* toAppearance(const Mat& param) const;
  vector<Mat> permutationOfParams() const;
  
  void overrideMeanShape(const MeshShape& newMeanShape);
  Rect getBound() const;
  // TAOTODO: Following is now conforming the state scaling and translation
  const double getMeanShapeScale() const { return this->meanShape.getScale(); };
};

class AAMPCA 
{
protected:
  ShapeModelPCA pcaShape;
  AppearanceModelPCA pcaAppearance;

public:
  AAMPCA(const ShapeModelPCA& shapePCA, const AppearanceModelPCA& appearancePCA)
    : pcaShape(shapePCA), pcaAppearance(appearancePCA) {};

  int dimensionShape() const { return pcaShape.dimension(); };
  int dimensionAppearance() const { return pcaAppearance.dimension(); };

  const ShapeModelPCA getShapePCA() const { return pcaShape; };
  const AppearanceModelPCA getAppearancePCA() const { return pcaAppearance; };
  inline unique_ptr<AAMPCA> clone() const
  {
    unique_ptr<AAMPCA> aam{ new AAMPCA(this->pcaShape, this->pcaAppearance) };
    return aam;
  };

  Rect getBound() const { return pcaAppearance.getBound(); };
};


#endif
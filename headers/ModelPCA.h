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
  ModelPCA(PCA& p) : pca(p), translation(Point2d(0,0)), scale(1) {};
  ModelPCA(PCA& p, const Point2d &t, const double s) : pca(p), translation(t), scale(s) {};
  inline virtual ~ModelPCA() {};

  // Encode a model to a parameter set (row vector)
  virtual Mat toParam(const BaseModel* m) const;
  virtual BaseModel* mean() const = 0;
  virtual BaseModel* toModel(const Mat& param) const = 0;

  // Generate permutation of parameter matrix
  virtual Mat* permutationOfParams() const = 0;

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
  ShapeModelPCA(PCA& p) : ModelPCA(p) {};
  ShapeModelPCA(PCA& p, const Point2d& t, const double s) : ModelPCA(p) {};
  BaseModel* mean() const; 
  BaseModel* toModel(const Mat& param) const;
  MeshShape* toShape(const Mat& param) const;
  Mat* permutationOfParams() const;
};

class AppearanceModelPCA : public ModelPCA 
{ 
protected:
  Size originalBound;
  MeshShape meanShape;
public: 
  AppearanceModelPCA() : ModelPCA() {};
  AppearanceModelPCA(PCA& p, const MeshShape& mean, const Size& size) : ModelPCA(p), originalBound(size) { this->meanShape = mean; };
  AppearanceModelPCA(const AppearanceModelPCA& that) : ModelPCA(that.pca) 
  { 
    originalBound = that.originalBound;
    meanShape = that.meanShape;
  };
  BaseModel* mean() const;
  
  const AppearanceModelPCA& cloneWithNewScale(double newScale, const Point2d& newTranslation) const;
  Mat toParam(const BaseModel* m) const;
  BaseModel* toModel(const Mat& param) const;
  Appearance* toAppearance(const Mat& param) const;
  Mat* permutationOfParams() const;
  
  void overrideMeanShape(const MeshShape& newMeanShape);
  Rect getBound() const;
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
};


#endif
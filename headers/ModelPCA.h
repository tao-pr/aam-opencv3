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
public:
  ModelPCA() {};
  ModelPCA(PCA& p) : pca(p) {};
  inline virtual ~ModelPCA() {};

  // Encode a model to a parameter set (row vector)
  virtual Mat toParam(const BaseModel* m) const;
  virtual BaseModel* mean() const = 0;
  virtual BaseModel* toModel(const Mat& param) const = 0;

  // Generate permutation of parameter matrix
  virtual Mat* permutationOfParams() const = 0;

  const int dimension() const { return this->pca.eigenvalues.rows; };
};

class ShapeModelPCA : public ModelPCA 
{ 
public: 
  ShapeModelPCA() : ModelPCA() {};
  ShapeModelPCA(PCA& p) : ModelPCA(p) {};
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
  AppearanceModelPCA(PCA& p, MeshShape mean, Size& size) : ModelPCA(p), originalBound(size) { this->meanShape = mean; };
  BaseModel* mean() const;
  
  Mat toParam(const BaseModel* m) const;
  BaseModel* toModel(const Mat& param) const;
  Appearance* toAppearance(const Mat& param) const;
  Mat* permutationOfParams() const;
  
  void overrideMeanShape(const MeshShape& newMeanShape);
  Rect getBound() const { return meanShape.getBound(); };
  
  const double getScale() const { return this->meanShape.getScale(); };
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
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
  ModelPCA(PCA& p) : pca(p) {};
  inline virtual ~ModelPCA() {};

  // Encode a model to a parameter set (row vector)
  virtual Mat toParam(const BaseModel* m) const;
  virtual BaseModel* mean() const = 0;
  virtual BaseModel* toModel(const Mat& param) const = 0;
};

class ShapeModelPCA : public ModelPCA 
{ 
public: 
  ShapeModelPCA(PCA& p) : ModelPCA(p) {};
  BaseModel* mean() const; 
  BaseModel* toModel(const Mat& param) const;
  MeshShape* toShape(const Mat& param) const;
};

class AppearanceModelPCA : public ModelPCA 
{ 
protected:
  MeshShape meanShape;
public: 
  AppearanceModelPCA(PCA& p, MeshShape mean) : ModelPCA(p) { this->meanShape = mean; };
  BaseModel* mean() const;
  BaseModel* toModel(const Mat& param) const;
  Appearance* toAppearance(const Mat& param) const;
};


#endif
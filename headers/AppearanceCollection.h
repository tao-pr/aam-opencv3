/**
 * Series of images for training process
 */

#ifndef APP_COLLECTION
#define APP_COLLECTION

#include "master.h"
#include "Appearance.h"
#include "BaseModel.h"
#include "MeshShape.h"
#include "ModelCollection.h"
#include "ShapeCollection.h"
#include "Params.h"

class AppearanceCollection : public ModelCollection
{
public:
  inline AppearanceCollection() : ModelCollection() {};
  AppearanceCollection(const vector<BaseModel*>& models) : ModelCollection(models) {};
  AppearanceCollection(const vector<Appearance*>& apps);
  AppearanceCollection(const AppearanceCollection& original);

  // ---------- Analysis -------------
  Mat covariance(const BaseModel* mean) const;
  void normaliseRotation();
  virtual double sumProcrustesDistance(const BaseModel* targetModel) const;
  
  // ---------- I/O ------------------
  Mat toMat() const;
  unique_ptr<ModelCollection> clone() const;
  unique_ptr<ModelCollection> toShapeCollection() const;
};

#endif
/**
 * Series of images for training process
 */

#ifndef APP_COLLECTION
#define APP_COLLECTION

#include "master.h"
#include "Appearance.h"
#include "BaseModel.h"
#include "ModelCollection.h"
#include "Params.h"

class AppearanceCollection : public ModelCollection
{
public:
  inline AppearanceCollection() : ModelCollection() {};
  AppearanceCollection(const vector<BaseModel*>& models, bool isVerbose=false) : ModelCollection(models, isVerbose) {};
  AppearanceCollection(const vector<Appearance*>& apps, bool isVerbose=false);
  AppearanceCollection(const AppearanceCollection& original);

  // ---------- Analysis -------------
  Mat covariance(const BaseModel* mean) const;
  unique_ptr<ModelCollection> normaliseRotation() const;
  
  // ---------- I/O ------------------
  Mat toMat() const;
  unique_ptr<ModelCollection> clone() const;
};

#endif
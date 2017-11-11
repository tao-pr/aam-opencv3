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
  AppearanceCollection(const vector<Appearance*>& apps, bool isVerbose=false);
  AppearanceCollection(const AppearanceCollection& original);

  // ---------- Analysis -------------
  Mat covariance(const BaseModel* mean) const;
  
  // ---------- I/O ------------------
  Mat toMat() const;
};

#endif
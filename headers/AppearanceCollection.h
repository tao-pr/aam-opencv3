/**
 * Series of images for training process
 */

#ifndef APP_COLLECTION
#define APP_COLLECTION

#include "master.h"
#include "AppearanceModel.h"
#include "ParamAppearance.h"

class AppearanceCollection 
{
private:
protected:
  vector<Appearance> items;
public:
  inline AppearanceCollection(){};
  inline AppearanceCollection(const vector<Appearance>& apps) : items(apps) {};
  inline AppearanceCollection(const AppearanceCollection& original) : items(original.items) {};

  inline void add(const Appearance &t){ this->items.push_back(t); };

  // ---------- Analysis of textures ------------
  Mat covariance(const Shape& mean) const;
  AppearanceEncoder pca(const Shape& meanShape) const;
  

  // ---------- Transformations ---------------
  AppearanceCollection clone(bool verbose=false) const;
  AppearanceCollection normaliseScalingTranslation() const;
  AppearanceCollection normaliseRotation() const;
  AppearanceCollection translateBy(const Point2d &p) const;

  // ---------- I/O ------------------
  inline vector<Appearance> getItems() const { return this->items; };
  Mat toMat() const;
};

#endif
/**
 * Series of images for training process
 */

#ifndef APP_COLLECTION
#define APP_COLLECTION

#include "master.h"
#include "AppearanceModel.h"

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
  

  // ---------- Transformations ---------------
  AppearanceCollection clone(bool verbose=false) const;
  AppearanceCollection normaliseScalingTranslation() const;
  AppearanceCollection normaliseRotation() const;
  AppearanceCollection translateBy(const Point2d &p) const;

  // ---------- I/O ------------------
  vector<Appearance> getItems() const;
  Mat toMat() const;
};

#endif
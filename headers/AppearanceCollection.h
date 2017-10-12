/**
 * Series of images for training process
 */

#ifndef APP_COLLECTION
#define APP_COLLECTION

#include "master.h"
#include "Texture.h"

class AppearanceCollection 
{
private:
protected:
  vector<Texture> items;
public:
  inline AppearanceCollection(){};
  inline AppearanceCollection(const vector<Texture>& apps) : items(apps) {};
  inline AppearanceCollection(const AppearanceCollection& original) : items(original.items) {};

  inline void add(const Texture &t){ this->items.push_back(t); };

  // ---------- Analysis of textures ------------
  

  // ---------- Transformations ---------------
  AppearanceCollection clone(bool verbose=false) const;
  AppearanceCollection normaliseScalingTranslation() const;
  AppearanceCollection normaliseRotation() const;
  AppearanceCollection translateBy(const Point2d &p) const;

  // ---------- I/O ------------------
  vector<Texture> getItems() const;
  Mat toMat() const;
};

#endif
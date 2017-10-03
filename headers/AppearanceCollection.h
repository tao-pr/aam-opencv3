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
};

#endif
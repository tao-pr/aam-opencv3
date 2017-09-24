/**
 * Series of images for training process
 */

#ifndef APP_COLLECTION
#define APP_COLLECTION

#include "master.h"

class AppearanceCollection 
{
private:
protected:
  vector<Mat> items;
public:
  inline AppearanceCollection(){};
  AppearanceCollection(const vector<Mat>& apps);
  AppearanceCollection(const AppearanceCollection& original);
};


#endif
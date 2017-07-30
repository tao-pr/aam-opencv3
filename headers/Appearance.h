/**
 * Appearance Model
 */

#ifndef APP_MODEL
#define APP_MODEL

#include "master.h"

class Appearance
{
protected:
  Point2D pos;
  Mat a; // [a] is usually a normalised (PCA) matrix from training image
public:
  Appearance();
  Appearance(const Point2D& p, const Mat& i);
  virtual ~Appearance();
  void setImage(const Mat& i);
  void setPos(const Point2D& p);
  Appearance operator +(const Appearance& that);
  Appearance operator *(const double scale);
  
  static Appearance createFromLinearCombination(Appearance baseApp, vector<Appearance> apps, vector<double> params);
};


#endif
/**
 * Appearance Model
 */

#ifndef APP_MODEL
#define APP_MODEL

#include "master.h"

class Appearance
{
protected:
  Point2d pos;
  Mat a; // [a] is usually a normalised (PCA) matrix from training image
public:
  Appearance(){};
  Appearance(const Point2d& p, const Mat& i);
  virtual ~Appearance(){};
  void setImage(const Mat& i);
  void setPos(const Point2d& p);
  // NOTE: The summation operator only averages the position
  // but will blend the appearance images altogether naively
  Appearance operator +(const Appearance& that) const;
  Appearance operator *(const double scale) const;
  
  static Appearance createFromLinearCombination(Appearance baseApp, vector<Appearance> apps, vector<double> params);
};


#endif
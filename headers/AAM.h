/**
 * Generic Active Appearance Model
 */

#ifndef GENERIC_AAM
#define GENERIC_AAM

#include "master.h"
#include "Shape.h"
#include "FaceLocaliser.h"

class AAM
{
private:
protected:
  FaceLocaliser faceFinder;
  Shape shape;
public:
  AAM(){};
  virtual ~AAM() {};

  size_t size() const { return this->shape.size(); };
  virtual void fit(Mat im) = 0;
  virtual void saveToFile(const string& modelFileName) const = 0;
  virtual void loadFromFile(const string& modelFileName) = 0;
};

#endif
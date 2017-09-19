/**
 * Generic Active Appearance Model
 */

#ifndef GENERIC_AAM
#define GENERIC_AAM

#include "master.h"
#include "PCAShape.h"
#include "FaceLocaliser.h"

class GenericAAM
{
private:
protected:
  FaceLocaliser* faceFinder;
  PCAShape shape;
public:
  GenericAAM(FaceLocaliser* faceDetector) { this->faceFinder = faceDetector; }
  virtual ~GenericAAM() {};

  size_t length() const { return this->shape.length(); };
  virtual void fit(Mat im) = 0;
  virtual void saveToFile(const string& modelFileName) const = 0;
  virtual void loadFromFile(const string& modelFileName) = 0;
  virtual void trainFromFileList(const vector<string>& filelist) = 0;
};


#endif
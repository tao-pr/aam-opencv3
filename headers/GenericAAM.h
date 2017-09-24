/**
 * Generic Active Appearance Model
 */

#ifndef GENERIC_AAM
#define GENERIC_AAM

#include "master.h"
#include "Shape.h"
#include "FaceLocaliser.h"
#include "ShapeCollection.h"
#include "AppearanceCollection.h"

/**
 * Representative of the trained AAM which is ready to use
 */
class GenericAAM
{
private:
protected:
  FaceLocaliser* faceFinder;

public:
  inline GenericAAM(FaceLocaliser* faceDetector) { this->faceFinder = faceDetector; }
  virtual inline ~GenericAAM() {};

  virtual void fit(Mat im) = 0;
  virtual void saveToFile(const string& modelFileName) const = 0;
  virtual void loadFromFile(const string& modelFileName) = 0;
  virtual void trainFromFileList(const vector<string>& filelist) = 0;
};

/**
 * Trainset collection for the AAM
 */
class GenericTrainset
{
private:
protected:
  ShapeCollection shapeEntries;
  AppearanceCollection appEntries;
public:
  GenericTrainset(const ShapeCollection &annotations, const AppearanceCollection &appearances);
  virtual inline ~GenericTrainset(){};
  virtual GenericAAM train(bool verbose) const = 0;
};


#endif
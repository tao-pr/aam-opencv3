/**
 * Generic Active Appearance Model
 */

#ifndef GENERIC_AAM
#define GENERIC_AAM

#include "master.h"
#include "MeshShape.h"
#include "FaceLocaliser.h"
#include "MeshShapeCollection.h"

/**
 * Representative of the trained AAM which is ready to use
 */
class GenericAAM
{
private:
protected:
  FaceLocaliser* faceFinder;
  MeshShape shape;
public:
  inline GenericAAM(FaceLocaliser* faceDetector) { this->faceFinder = faceDetector; }
  virtual ~GenericAAM() {};

  size_t inline length() const { return this->shape.length(); };
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
  MeshShapeCollection shapeEntries;
public:
  GenericTrainset(const MeshShapeCollection &annotations, const vector<Mat> &imgs);
  virtual inline ~GenericTrainset(){};
  virtual GenericAAM train(bool verbose) const;
};


#endif
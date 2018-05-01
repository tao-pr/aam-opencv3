/**
 * Generic Active Appearance Model
 */

#ifndef GENERIC_AAM
#define GENERIC_AAM

#include "master.h"
#include "MeshShape.h"
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
  MeshShape shape;
  vector<Texture> textures;

public:
  inline GenericAAM(FaceLocaliser* faceDetector) { this->faceFinder = faceDetector; }
  virtual inline ~GenericAAM() {};

  virtual void fit(Mat im) = 0;
  virtual void saveToFile(const string& modelFileName) const = 0;
  virtual void loadFromFile(const string& modelFileName) = 0;
  virtual void trainFromFileList(const vector<string>& filelist) = 0;

  inline MeshShape getShape() const { return this->shape; };
  inline vector<Texture> getTextures() const { return this->textures; };
};

#endif
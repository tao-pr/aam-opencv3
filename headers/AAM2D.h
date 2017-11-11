/**
 * Two dimensional Active Appearance Model
 */

#ifndef AAM_2DMODEL
#define AAM_2DMODEL

#include "master.h"
#include "IO.h"
#include "GenericAAM.h"
#include "FaceLocaliser.h"

class AAM2D : public GenericAAM
{
private:
protected:
public:
  inline AAM2D(FaceLocaliser* faceDetector) : GenericAAM(faceDetector){};
  virtual inline ~AAM2D(){};
  void fit(Mat im);
  void saveToFile(const string& modelFileName) const;
  void loadFromFile(const string& modelFileName);
  void trainFromFileList(const vector<string>& filelist);
};

// class Trainset2D : public GenericTrainset
// {
// private:
// protected:
// public:
//   GenericAAM train(bool verbose) const;
// };


#endif
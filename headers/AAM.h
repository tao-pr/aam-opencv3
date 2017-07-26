/**
 * Generic Active Appearance Model
 */

#ifndef GENERIC_AAM
#define GENERIC_AAM

#include "master.h"
#include "FaceLocaliser.h"

class AAM
{
private:
protected:
  FaceLocaliser faceFinder;
public:
  AAM();
  AAM(const string& modelFileName);
  virtual ~AAM();

  void fit(Mat im);
  void saveToFile(const string& modelFileName) const;
  void loadFromFile(const string& modelFileName);
};

#endif
/**
 * Generic Face Localiser
 */

#ifndef FACE_LOCALISER
#define FACE_LOCALISER

#include "master.h"

class FaceLocaliser 
{
private:
public:
  virtual ~FaceLocaliser() {};
  virtual vector<Rect> find(Mat im) const = 0;
};

class HaarBasedFaceLocaliser : public FaceLocaliser
{
public:
  virtual ~FaceLocaliser() {};
  virtual vector<Rect> find(Mat im) const;
};


#endif
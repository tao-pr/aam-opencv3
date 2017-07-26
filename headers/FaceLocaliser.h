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
  virtual ~FaceLocaliser();
  Rect find(Mat im) const = 0;
};


#endif
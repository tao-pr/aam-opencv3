/**
 * Two dimensional Active Appearance Model
 */

#ifndef AAM2D
#define AAM2D

#include "master.h"
#include "GenericAAM.h"

class Trainset2D : public GenericTrainset
{
private:
protected:
public:
  GenericAAM train(bool verbose) const;
};


#endif
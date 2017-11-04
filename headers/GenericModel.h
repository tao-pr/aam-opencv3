/**
 * Generic Statistical Model
 * Base of Appearance / Shape model
 */

#ifndef GENERIC_MODEL
#define GENERIC_MODEL

#include "master.h"

class GenericModel
{
private:
public:
  virtual ~GenericModel(){};
  virtual Mat toRowVector() const = 0;
  virtual Mat toColVector() const = 0;

  virtual void save(const string path) const = 0;
  virtual void load(const string path) = 0;
};

#endif
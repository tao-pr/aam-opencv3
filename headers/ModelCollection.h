#ifndef GENERIC_COLLECTION
#define GENERIC_COLLECTION

#include "master.h"
#include "Params.h"
#include "BaseModel.h"

struct Alignment;

class ModelCollection 
{
private:
protected:
  bool verbose;
  vector<BaseModel*> items;
public:
  ModelCollection() : verbose(false) {};
  ModelCollection(vector<BaseModel*> vs, bool verbose=false) : items(vs), verbose(verbose) {};
  virtual ~ModelCollection();

  inline void add(BaseModel* item){ this->items.push_back(item); };
  virtual ModelCollection clone() const = 0;

  // ------- Geometrical Analysis -------------
  Alignment procrustesMean(double tol=1e-3, int maxIter=10) const;
  double sumProcrustesDistance(const BaseModel* targetModel) const;
  Mat covariance(const BaseModel* mean) const;
  ModelEncoder pca(const BaseModel* mean) const;

};

struct Alignment 
{
  ModelCollection collection;
  BaseModel mean;

  Alignment(ModelCollection vs, BaseModel _mean) : collection(vs), mean(_mean) {};
};

#endif
#ifndef GENERIC_COLLECTION
#define GENERIC_COLLECTION

#include "master.h"
#include "Params.h"
#include "BaseModel.h"

class ModelCollection;

typedef tuple<unique_ptr<ModelCollection>, BaseModel*> ModelAlignment;

class ModelCollection 
{
private:
protected:
  bool verbose;
  vector<BaseModel*> items;
public:
  ModelCollection(bool verbose=false) : verbose(verbose) {};
  ModelCollection(vector<BaseModel*> vs, bool verbose=false) : items(vs), verbose(verbose) {};
  virtual ~ModelCollection();

  inline void add(BaseModel* item){ this->items.push_back(item); };
  virtual unique_ptr<ModelCollection> clone() const = 0;

  // ------- Geometrical Analysis -------------
  ModelAlignment procrustesMean(double tol=1e-3, int maxIter=10) const;
  double sumProcrustesDistance(const BaseModel* targetModel) const;
  Mat covariance(const BaseModel* mean) const;
  ModelEncoder pca(const BaseModel* mean) const;

};

#endif
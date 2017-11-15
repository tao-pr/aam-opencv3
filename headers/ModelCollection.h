#ifndef GENERIC_COLLECTION
#define GENERIC_COLLECTION

#include "master.h"
#include "Params.h"
#include "BaseModel.h"

class ModelCollection;

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
  virtual void clear();

  inline void add(BaseModel* item){ this->items.push_back(item); };
  virtual unique_ptr<ModelCollection> clone() const = 0;
  virtual Mat toMat() const = 0;
  inline const vector<BaseModel*> getItems() const { return this->items; };

  // ------- Geometrical Analysis -------------
  virtual BaseModel* procrustesMean(double tol=1e-3, int maxIter=10);
  virtual double sumProcrustesDistance(const BaseModel* targetModel) const;
  virtual void normaliseRotation();
  virtual Mat covariance(const BaseModel* mean) const;
  virtual ModelEncoder pca(const BaseModel* mean) const;

};

#endif
#ifndef GENERIC_COLLECTION
#define GENERIC_COLLECTION

#include "master.h"
#include "Params.h"
#include "BaseModel.h"
#include "ModelPCA.h"

class ModelCollection 
{
private:
  long long uid;
  static long long generateUID();
protected:
  vector<BaseModel*> items;
  long long getUID() const { return this->uid; };
public:
  ModelCollection() : uid(ModelCollection::generateUID()) {};
  ModelCollection(vector<BaseModel*> vs) : uid(ModelCollection::generateUID()), items(vs) {};
  virtual ~ModelCollection();
  virtual void clear();

  inline void add(BaseModel* item){ this->items.push_back(item); };
  virtual unique_ptr<ModelCollection> clone() const = 0;
  virtual Mat toMat() const = 0;
  inline const vector<BaseModel*> getItems() { return this->items; };
  inline int size() const { return this->items.size(); };

  // ------- Geometrical Analysis -------------
  virtual BaseModel* procrustesMean(double tol=1e-3, int maxIter=10);
  virtual double sumProcrustesDistance(const BaseModel* targetModel) const;
  virtual void normaliseRotation();
  virtual Mat covariance(const BaseModel* mean) const;
  virtual ModelPCA* pca(const BaseModel* mean, int maxDimension) const;

};

#endif
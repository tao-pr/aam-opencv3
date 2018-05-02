#ifndef TREE_SEARCH
#define TREE_SEARCH

#include "master.h"
#include "BaseModel.h"
#include "Shape.h"
#include "MeshShape.h"
#include "Appearance.h"
#include "ModelPCA.h"
#include "BaseFittedModel.h"
#include "ModelFitter.h"
#include "BaseFittedModel.h"

class TreeSearch 
{
private:
protected:
  unique_ptr<BaseFittedModel> fitter;
  vector<unique_ptr<TreeSearch>> branches;
  double bestError;
  bool isOrphan;

public:
  TreeSearch();
  TreeSearch(const unique_ptr<BaseFittedModel> &fitter);
  virtual ~TreeSearch(){};

  // Properties
  unique_ptr<BaseFittedModel> get() const;
  bool isTerminal() const;
  const double getBestError(){ return this->bestError; } const;

  // Methods
  unique_ptr<TreeSearch> clone() const;
  unique_ptr<TreeSearch> expandBranches() const;
  void prune(double decayRatio = 0.667);

};

#endif
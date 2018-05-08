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
  vector<unique_ptr<TreeSearch>> branches;
  double bestError;
  double error; // Error of current model
  bool isOrphan;

public:
  unique_ptr<BaseFittedModel> model;

  TreeSearch();
  TreeSearch(unique_ptr<BaseFittedModel> model, double error);
  virtual ~TreeSearch(){};

  // Properties
  bool isTerminal() const;
  const double getBestError() const { return this->bestError; };
  const int getDepth() const;

  // Methods
  unique_ptr<TreeSearch> expandBranches(unique_ptr<ModelFitter> &fitter, const Mat& sample) const;
  bool prune(double decayRatio = 0.667);
  void deleteAllBranches();

};

#endif
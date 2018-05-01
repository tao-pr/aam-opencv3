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

class TreeSearch 
{
private:
protected:
  unique_ptr<ModelFitter> fitter;
  vector<unique_ptr<TreeSearch>> branches;
public:
  TreeSearch();
  TreeSearch(const unique_ptr<ModelFitter> &fitter);
  virtual ~TreeSearch(){};

  unique_ptr<ModelFitter> get() const;

  unique_ptr<TreeSearch> clone() const;
  unique_ptr<TreeSearch> generateNextRoutes() const;
  void prune();
  bool isTerminal() const;
  const unique_ptr<TreeSearch>
};

#endif
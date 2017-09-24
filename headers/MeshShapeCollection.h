/**
 * Series of MeshShape for training process
 */

#ifndef MESHSHAPE_COLLECTION
#define MESHSHAPE_COLLECTION

#include "master.h"
#include "MeshShape.h"

class MeshShapeCollection 
{
private:
protected:
  vector<MeshShape> items;
public:
  inline MeshShapeCollection(){};
  MeshShapeCollection(const vector<MeshShape>& shapes);

  inline void add(MeshShape s){ this->items.push_back(s); };

  MeshShape mean() const;
};




#endif
#ifndef PRIORITY_LLIST
#define PRIORITY_LLIST

#include "master.h"
#include "BaseFittedModel.h"

template <class T>
class PriorityLinkedList
{
public:
  inline PriorityLinkedList(const PriorityLinkedList<T>& ) = delete;
  inline PriorityLinkedList() : v(0), ptr(nullptr), next(nullptr) {};
  inline PriorityLinkedList(unique_ptr<T> t, double v) 
    : v(v), ptr(move(t)), next(nullptr) {};
  virtual inline ~PriorityLinkedList(){};

  double v;
  unique_ptr<T> ptr;
  unique_ptr<PriorityLinkedList<T>> next;

  virtual void push(unique_ptr<T>& n, double v);
  virtual bool pop();
  void take(int n);
  int size() const;

  // TAOTOREVIEW: add cout 
};


#endif
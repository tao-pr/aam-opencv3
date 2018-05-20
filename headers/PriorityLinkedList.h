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

  virtual void push(unique_ptr<T>& n, double d)
  {
    if (this->ptr == nullptr)
    {
      // Itself empty, assign in-place
      this->ptr = move(n);
      this->v = d;
    }
    else
    {
      if (this->v >= d)
      {
        // Insert an intermediate node here
        unique_ptr<PriorityLinkedList<T>> nn{new PriorityLinkedList(move(n), d)};
        nn->next = move(this->next);
        this->next = move(nn);
      }
      else
      {
        // Add to next
        if (this->next != nullptr)
          this->next->push(n, d);
        else
        {
          unique_ptr<PriorityLinkedList<T>> nn{new PriorityLinkedList(move(n), d)};
          this->next = move(nn);
        }
      }
    }
  };

  virtual bool clear()
  {
    this->ptr = nullptr;
    if (this->next != nullptr)
      this->next->clear();
  };

  void take(int n)
  {
    if (this->ptr == nullptr) return;
    else 
    {
      if (this->next)
        this->next->take(n-1);

      if (n <= 0)
      {
        if (this->ptr != nullptr) this->ptr.release();
        if (this->next != nullptr) 
        {
          this->next->clear();
          this->next.release();
        }
      }
    }
  };

  int size() const
  {
    if (this->ptr == nullptr) return 0;
    else if (this->next == nullptr) return 1;
    else return 1 + this->next->size();
  };

  virtual void printValueList(string prefix) const
  {
    cout << prefix;
    if (this->ptr)
      cout << this->v;
    if (this->next != nullptr)
    {
      cout << ", ";
      this->next->printValueList("");
    }
    else cout << endl;
  };

  // TAOTOREVIEW: add cout 
};


#endif
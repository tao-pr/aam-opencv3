#ifndef PRIORITY_LLIST
#define PRIORITY_LLIST

#include "master.h"
#include "BaseFittedModel.h"

// TAOTODO: Make this thread safe

template <class T>
class PriorityLinkedList
{
private:
  mutex mx;
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
      this->next = nullptr;
    }
    else
    {
      if (this->v >= d)
      {
        // Insert an intermediate node here
        unique_ptr<PriorityLinkedList<T>> asNext{new PriorityLinkedList(move(ptr), v)};
        asNext->next = move(this->next);
        this->ptr = move(n);
        this->v = d;
        this->next = move(asNext);
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
    else if (this->next != nullptr)
    {
      if (n <= 0)
      {
        this->ptr.release();
        this->ptr = nullptr;
      }
      this->next->take(n-1);
      
      if (n <= 1)
      {
        this->next.release();
        this->next = nullptr;
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
};


#endif
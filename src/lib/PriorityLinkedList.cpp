#include "PriorityLinkedList.h"

template<class T>
void PriorityLinkedList<T>::push(unique_ptr<T> n, double v)
{
  // TAOTODO:
  if (this->next == nullptr)
  {

  }
  else
  {

  }
}

template<class T>
void PriorityLinkedList<T>::take(int n)
{
  if (this->ptr == nullptr) return;
  else 
  {
    if (this->next)
      this->next->take(n-1);

    if (n <= 0)
    {
      if (this->ptr) this->ptr.release();
      if (this->next) this->next.release();
    }
  }
}
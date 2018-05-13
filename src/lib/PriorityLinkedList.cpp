#include "PriorityLinkedList.h"

template<class T>
void PriorityLinkedList<T>::push(unique_ptr<T>& n, double v)
{
  if (this->ptr == nullptr)
  {
    // Itself empty, assign in-place
    this->ptr = move(n);
    this->v = v;
  }
  else
  {
    if (this->v >= v)
    {
      // Insert an intermediate node here
      unique_ptr<PriorityLinkedList<T>> nn{new PriorityLinkedList(n, v)};
      nn->next = move(this->next);
      this->next = move(nn);
    }
    else
    {
      // Add to next
      if (this->next)
        this->next->push(n, v);
      else
      {
        unique_ptr<PriorityLinkedList<T>> nn{new PriorityLinkedList(n, v)};
        this->next = move(nn);
      }
    }
  }
}

template<class T>
bool PriorityLinkedList::pop()
{
  if (this->ptr)
  {
    if (this->next)
    {
      this->ptr = move(this->next->ptr);
    }
    else this->ptr.release();

    return true;
  }
  else return false;
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

template<class T>
int PriorityLinkedList<T>::size() const
{
  if (this->ptr == nullptr) return 0;
  else if (this->next == nullptr) return 1;
  else return 1 + this->next.size();
}

template<class T>
void PriorityLinkedList<T>::iter(function<void (T)> f) const
{
  if (this->ptr == nullptr) return;
  else 
  {
    f(ptr);
    this->next->iter(f);
  }
}
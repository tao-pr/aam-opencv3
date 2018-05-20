#include "PriorityLinkedList.h"

template<class T>
void PriorityLinkedList<T>::push(unique_ptr<T>& n, double v)
{
  #ifdef DEBUG_PRIORITY_LIST
  cout << "-> push" << endl;
  #endif
  if (this->ptr == nullptr)
  {
    // Itself empty, assign in-place
    #ifdef DEBUG_PRIORITY_LIST
    cout << "-> push self" << endl;
    #endif
    this->ptr = move(n);
    this->v = v;
  }
  else
  {
    if (this->v <= v)
    {
      // Insert an intermediate node here
      #ifdef DEBUG_PRIORITY_LIST
      cout << "-> push insert" << endl;
      #endif
      unique_ptr<PriorityLinkedList<T>> nn{new PriorityLinkedList(move(n), v)};
      nn->next = move(this->next);
      this->next = move(nn);
    }
    else
    {
      // Add to next
      #ifdef DEBUG_PRIORITY_LIST
      cout << "-> push next" << endl;
      #endif
      if (this->next)
        this->next->push(n, v);
      else
      {
        unique_ptr<PriorityLinkedList<T>> nn{new PriorityLinkedList(move(n), v)};
        this->next = move(nn);
      }
    }
  }
}

template<class T>
bool PriorityLinkedList<T>::clear()
{
  while (this->pop());
  this->ptr = nullptr;
}

template<class T>
bool PriorityLinkedList<T>::pop()
{
  if (this->ptr)
  {
    if (this->next)
    {
      // Remove head, and next becomes this
      this->ptr = move(this->next->ptr);
      if (this->next->next)
      {
        auto vn = next->next.get();
        this->next.reset(vn);
      }
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
void PriorityLinkedList<T>::printValueList(string prefix) const
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
}

template<class T>
int PriorityLinkedList<T>::size() const
{
  if (this->ptr == nullptr) return 0;
  else if (this->next == nullptr) return 1;
  else return 1 + this->next->size();
}

template class PriorityLinkedList<BaseFittedModel>;

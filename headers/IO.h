#ifndef IO_H
#define IO_H

#include "master.h"

namespace IO
{
  class GenericIO
  {
  private:
  public:
    virtual ~GenericIO(){}
    virtual void render(const Mat& im) const = 0;
  };

  class FileOutputIO : GenericIO
  {
  private:
    string fileName;
  public:
    FileOutputIO(const string fileName){ this->fileName = fileName; }
    void render(const Mat& im) const 
    {
      // TAOTODO:
    }
  };

  class WindowIO : GenericIO
  {
  private:
    string wndName;
  public:
    WindowIO(const string wndName){ this->wndName = wndName; }
    void render(const Mat& im) const 
    {
      imshow(this->wndName, im);
    }
  };
};


#endif
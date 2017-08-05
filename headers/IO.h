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
    virtual void render(const Mat& im) = 0;
  };

  class FileOutputIO : GenericIO
  {
  protected:
    string fileName;
  public:
    FileOutputIO(const string fileName){ this->fileName = fileName; }
    virtual void render(const Mat& im) 
    {
      imwrite(fileName, im);
    }
  };

  class FileOutputWithRunningNameIO : FileOutputIO
  {
  private:
    string baseFileName;
    long long n;
  protected:
    void runFileName()
    {
      this->n++;
      this->fileName = fmt::format(
        "{0}-{1:4d}.jpg", 
        this->baseFileName, this->n);
    }
  public:
    FileOutputWithRunningNameIO(const string baseFileName)
    : FileOutputIO(baseFileName)
    {
      this->baseFileName = baseFileName;
      this->n            = -1;
      this->runFileName();
    }

    virtual void render(const Mat& im)
    {
      FileOutputIO::render(im);
      this->runFileName();
    }
  };

  class WindowIO : GenericIO
  {
  private:
    string wndName;
  public:
    WindowIO(const string wndName){ this->wndName = wndName; }
    virtual void render(const Mat& im)
    {
      imshow(this->wndName, im);
    }
  };
};


#endif
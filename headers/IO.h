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

  class FileOutputIO : public GenericIO
  {
  protected:
    string fileName;
  public:
    FileOutputIO(const string fileName){ this->fileName = fileName; }
    inline void render(const Mat& im) 
    {
      imwrite(fileName, im);
    }
  };

  class FileOutputWithRunningNameIO : public FileOutputIO
  {
  private:
    string baseFileName;
    long long n;
  protected:
    inline void runFileName()
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

    inline void render(const Mat& im)
    {
      FileOutputIO::render(im);
      this->runFileName();
    }
  };

  class WindowIO : public GenericIO
  {
  private:
    string wndName;
    Point pos;
  public:
    WindowIO(const string wndName, const Point p = Point(0,0)){ this->wndName = wndName; this->pos = pos; }
    inline void render(const Mat& im)
    {
      imshow(this->wndName, im);
      moveWindow(this->wndName, this->pos.x, this->pos.y);
    }
  };

  class MatIO : public GenericIO
  {
  private:
    Mat m;
  public:
    MatIO(){};

    inline void render(const Mat& im)
    {
      im.copyTo(this->m);
    }
    
    Mat get()
    {
      return m;
    }
  };
};


#endif
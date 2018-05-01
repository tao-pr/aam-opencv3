/**
 * Master.h is supposed to be included by all header files 
 * inside the project
 */

#ifndef MASTER_H
#define MASTER_H

#include <string>
#include <vector>
#include <tuple>
#include <stack>
#include <stdlib.h>
#include <limits>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <algorithm>
#include <fmt/format.h>
#include <stdio.h>
#include <dirent.h>
#include <chrono>
#include <sys/resource.h>

// Following imports are for stacktracing
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifdef DEBUG
#include "backward.h"
#endif

using namespace cv;
using namespace std;
using namespace backward;

extern StackTrace stacktrace;

const std::string RED("\033[0;31m");
const std::string GREEN("\033[1;32m");
const std::string YELLOW("\033[1;33m");
const std::string CYAN("\033[0;36m");
const std::string MAGENTA("\033[0;35m");
const std::string RESET("\033[0m");

inline void adjustStackSize()
{
  const rlim_t minStackSize = 128 * 1024 * 1024; // 128 MB
  struct rlimit lim;
  int result = getrlimit(RLIMIT_STACK, &lim);
  if (result == 0)
  {
    if (lim.rlim_cur < minStackSize)
    {
      #ifdef DEBUG
      cout << MAGENTA << "[Overriding stack size to "<< minStackSize/(1024*1024) << " MB]" 
        << RESET << " (formerly " << lim.rlim_cur/(1024*1024) << " MB)" << endl;
      #endif

      lim.rlim_cur = minStackSize;
      result = setrlimit(RLIMIT_STACK, &lim);
      if (result != 0)
      {
        cout << YELLOW << "[WARNING] unable to override stack size" << RESET << endl;
      }
    }
    else
    {
      #ifndef DEBUG
      cout << MAGENTA << "[Stack size remains " << lim.rlim_cur / (1024*1024) << " MB]" << RESET << endl;
      #endif
    }
  }
}

// @href https://stackoverflow.com/questions/77005/how-to-automatically-generate-a-stacktrace-when-my-gcc-c-program-crashes
inline void segFaultHandler(int sig) 
{
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

#endif
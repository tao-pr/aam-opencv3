#ifndef AUX_UTIL
#define AUX_UTIL

#include "master.h"

namespace Aux
{
  Mat inline scaler(const Mat& m, double multiplier)
  {
    if (multiplier == 1.0) return m;
    else {
      int rows = (int)ceil(m.rows * multiplier);
      int cols = (int)ceil(m.cols * multiplier);
      Mat out  = Mat(rows, cols, m.type());
      resize(m, out, Size(rows, cols));
      return out;
    }
  }
}

#endif
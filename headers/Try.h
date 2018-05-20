#ifndef TRY_UTIL
#define TRY_UTIL

#include "master.h"

#define TRY try{
#define END_TRY }catch(Exception* e){ cerr << e << endl; }

#endif
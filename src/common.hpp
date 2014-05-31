#ifndef _INCLUDE_COMMON_HPP
#define _INCLUDE_COMMON_HPP

#include <mpi.h>

#include <cstdio>

#include <mutex>
#include <thread>
#include <queue>
#include <vector>
#include <map>

using namespace std;

typedef enum {
  WINDMILL = 0,
  HORSE
} Resource;

#endif

#ifndef _INCLUDE_COMMON_HPP
#define _INCLUDE_COMMON_HPP

#include <mpi.h>

#include <signal.h>

#include <cstdio>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <random>

#include <queue>
#include <vector>
#include <map>

using namespace std;

typedef enum {
  WINDMILL = 0,
  HORSE
} Resource;

#endif

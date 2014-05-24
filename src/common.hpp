#ifndef _INCLUDE_COMMON_HPP
#define _INCLUDE_COMMON_HPP

#include <mutex>
#include <thread>

using namespace std;

#define SIDES_NUM 4
#define RES_NUM   2

enum state {
  IDLE = 0,
  HAS_TOKEN
};

enum side {
  SELF = 0,
  LEFT,
  RIGHT,
  PARENT
};

enum resource {
  WINDMILL = 0,
  HORSE
};

#endif

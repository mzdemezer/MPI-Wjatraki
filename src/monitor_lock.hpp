#ifndef _INCLUDE_MONITOR_LOCK_HPP
#define _INCLUDE_MONITOR_LOCK_HPP

#include "common.hpp"

class MonitorLock {
public:
  virtual ~MonitorLock() {}
  virtual void reserve(Resource resource) = 0;
  virtual void release(Resource resource) = 0;
  virtual unsigned get_index() = 0;
};

#endif

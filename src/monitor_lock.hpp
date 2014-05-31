#ifndef _INCLUDE_MONITOR_LOCK_HPP
#define _INCLUDE_MONITOR_LOCK_HPP

class MonitorLock {
public:
  void reserve() = 0;
  void release() = 0;
};

#endif

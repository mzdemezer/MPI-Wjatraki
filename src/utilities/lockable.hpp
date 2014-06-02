#ifndef _INCLUDE_LOCKABLE_HPP
#define _INCLUDE_LOCKABLE_HPP

#include "../common.hpp"

class Lockable {
private:
  mutex mtx;
  condition_variable cv;
  unique_lock<mutex> lck;

public:
  Lockable();
  virtual ~Lockable();

  void lock();
  void unlock();
  void wait();
  void notify();
};

#endif

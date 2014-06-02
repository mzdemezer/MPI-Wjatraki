#include "lockable.hpp"

Lockable::~Lockable() {}

void Lockable::lock() {
  mtx.lock();
}

void Lockable::unlock() {
  mtx.unlock();
}

void Lockable::wait() {
  unique_lock<mutex> lck(mtx, defer_lock);
  if (DEBUG) printf("\t\twait\n");
  cv.wait(lck);
  if (DEBUG) printf("\t\twoken up\n");
}

void Lockable::notify() {
  cv.notify_one();
}

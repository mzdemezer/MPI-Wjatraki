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
  cv.wait(lck);
}

void Lockable::notify() {
  cv.notify_one();
}

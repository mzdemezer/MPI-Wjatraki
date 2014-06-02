#include "lockable.hpp"

Lockable::Lockable()
: lck(mtx, defer_lock)
{}

virtual ~Lockable() {}

void Lockable::lock() {
  lck.lock();
}

void Lockable::unlock() {
  lck.unlock();
}

void Lockable::wait() {
  cv.wait(lck);
}

void Lockable::notify() {
  cv.notify_one();
}

#include "common.hpp"

unsigned random_unsigned(unsigned from, unsigned to) {
  default_random_engine generator;
  uniform_int_distribution<unsigned> distribution(from, to);
  return distribution(generator);
}

void random_sleep(unsigned min_ms, unsigned max_ms) {
  unsigned sleep_time = random_unsigned(min_ms, max_ms);
  this_thread::sleep_for(chrono::milliseconds(sleep_time));
}

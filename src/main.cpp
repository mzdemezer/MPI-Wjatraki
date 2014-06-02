#include "common.hpp"
#include "mpi_lock.hpp"
#include "mpi_controller.hpp"

#define TREE_RANK 2
#define NUMBER_OF_HORSES 1
#define NUMBER_OF_WINDMILLS 1

bool should_work = true;

void stop_work(int sig) {
  should_work = false;
}

unsigned random_unsigned(unsigned from, unsigned to) {
  default_random_engine generator;
  uniform_int_distribution<unsigned> distribution(from, to);
  return distribution(generator);
}

void random_sleep(unsigned min_ms, unsigned max_ms) {
  unsigned sleep_time = random_unsigned(min_ms, max_ms);
  this_thread::sleep_for(chrono::milliseconds(sleep_time));
}

void application_process(MonitorLock *lock) {
  while (should_work) {
    lock->reserve(HORSE);
    lock->reserve(WINDMILL);

    if (DEBUG) printf("#%u: fighting\n", lock->get_index());
    random_sleep(400, 1000);

    lock->release(HORSE);
    lock->release(WINDMILL);

    if (DEBUG) printf("#%u: thinking\n", lock->get_index());
    random_sleep(200, 1500);
  }
}

void monitor_process(MPIController &controller) {
  while (should_work) {
    controller.handle_message();
  }
}

int main(int argc, char **argv) {
  thread application_thread;
  MPIController controller(&argc, &argv, (unsigned)TREE_RANK);

  controller.add_resource(HORSE, NUMBER_OF_HORSES);
  controller.add_resource(WINDMILL, NUMBER_OF_WINDMILLS * 4);

  // signal(SIGINT, stop_work);

  application_thread = thread(application_process, controller.get_lock());
  monitor_process(controller);
  application_thread.join();

  return 0;
}

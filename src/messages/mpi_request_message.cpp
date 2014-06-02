#include "mpi_request_message.hpp"
#include "../mpi_lock.hpp"

void MPIRequestMessage::deliver(unsigned sender, MPILock &resource_lock) {
  resource_lock.receive_request(sender, *this);
}

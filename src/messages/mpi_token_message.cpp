#include "mpi_token_message.hpp"
#include "../mpi_lock.hpp"

void MPITokenMessage::deliver(unsigned sender, MPILock &resource_lock) {
  resource_lock.receive_token(sender, *this);
}

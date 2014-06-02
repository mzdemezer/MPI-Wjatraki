#ifndef _INCLUDE_MPI_RECEIVE_INTERFACE_HPP
#define _INCLUDE_MPI_RECEIVE_INTERFACE_HPP

#include "common.hpp"
#include "mpi_lock.hpp"

class MPIReceiveInterface {
public:
  void receive_message(MPILock &resource_lock) = 0;
};

#endif

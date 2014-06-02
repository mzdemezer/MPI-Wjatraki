#ifndef _INCLUDE_MPI_REQUEST_MESSAGE_HPP
#define _INCLUDE_MPI_REQUEST_MESSAGE_HPP

class MPIRequestMessage;
class MPILock;

#include "../common.hpp"
#include "mpi_message.hpp"

class MPIRequestMessage : public MPIMessage {
public:
  Resource resource;

  MPIRequestMessage(Resource resource)
  : resource(resource)
  {}

  MPIRequestMessage(const MPIRequestMessage &other)
  : resource(other.resource) {}

  void deliver(unsigned sender, MPILock &resource_lock);
};

#endif

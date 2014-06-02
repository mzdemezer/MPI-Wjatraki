#ifndef _INCLUDE_MPI_TOKEN_MESSAGE_HPP
#define _INCLUDE_MPI_TOKEN_MESSAGE_HPP

class MPITokenMessage;
class MPILock;

#include "../common.hpp"
#include "mpi_message.hpp"

class MPITokenMessage : public MPIMessage {
public:
  Resource resource;
  bool send_back;

  MPITokenMessage(Resource resource, bool send_back)
  : resource(resource)
  , send_back(send_back)
  {}

  MPITokenMessage(const MPITokenMessage &other)
  : resource(other.resource)
  , send_back(other.send_back) {}

  void deliver(unsigned sender, MPILock &resource_lock);
};

#endif

#ifndef _INCLUDE_MPI_SEND_INTERFACE_HPP
#define _INCLUDE_MPI_SEND_INTERFACE_HPP

class MPISendInterface;

#include "common.hpp"
#include "messages/mpi_token_message.hpp"
#include "messages/mpi_request_message.hpp"
#include "mpi_lock.hpp"

class MPISendInterface {
public:
  virtual void send_token(unsigned receiver, MPITokenMessage *message) = 0;
  virtual void send_request(unsigned receiver, MPIRequestMessage *message) = 0;
};

#endif

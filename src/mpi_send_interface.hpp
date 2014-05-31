#ifndef _INCLUDE_MPI_SEND_INTERFACE_HPP
#define _INCLUDE_MPI_SEND_INTERFACE_HPP

#include "common.hpp"
#include "mpi_token_message.hpp"

class MPISendInterface {
public:
  void send_token(unsigned receiver, Resource resource, MPITokenMessage &message) = 0;
  void send_request(unsigned receiver, Resource resource) = 0;
};

#endif

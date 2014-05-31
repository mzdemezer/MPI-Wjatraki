#ifndef _INCLUDE_MPI_REQUEST_MESSAGE_HPP
#define _INCLUDE_MPI_REQUEST_MESSAGE_HPP

#include "common.hpp"

class MPIRequestMessage {
public:
  Resource resource;

  MPIRequestMessage(Resource resource)
  : resource(resource)
  {}
};

#endif

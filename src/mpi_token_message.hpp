#ifndef _INCLUDE_MPI_TOKEN_MESSAGE_HPP
#define _INCLUDE_MPI_TOKEN_MESSAGE_HPP

#include "common.hpp"

class MPITokenMessage {
public:
  Resource resource;
  bool send_back;

  MPITokenMessage(Resource resource, bool send_back)
  : resource(resource)
  , send_back(send_back)
  {}
};

#endif

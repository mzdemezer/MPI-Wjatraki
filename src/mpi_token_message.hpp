#ifndef _INCLUDE_MPI_TOKEN_MESSAGE_HPP
#define _INCLUDE_MPI_TOKEN_MESSAGE_HPP

#include "common.hpp"

class MPITokenMessage {
public:
  bool send_back;
  MPITokenMessage(bool send_back) : send_back(send_back) {}
};

#endif

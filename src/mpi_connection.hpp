#ifndef _INCLUDE_MPI_CONNECTION_HPP
#define _INCLUDE_MPI_CONNECTION_HPP

#include "common.hpp"
#include "mpi_send_interface.hpp"
#include "mpi_receive_interface.hpp"

class MPIConnection : public MPISendInterface, public MPIReceiveInterface {
private:
  void send_message(unsigned receiver, void *message, int message_size, int tag);

public:
  void send_token(unsigned receiver, MPITokenMessage *message);
  void send_request(unsigned receiver, MPIRequestMessage *message);
};

#endif

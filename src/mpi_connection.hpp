#ifndef _INCLUDE_MPI_CONNECTION_HPP
#define _INCLUDE_MPI_CONNECTION_HPP

class MPIConnection;

#include "common.hpp"
#include "mpi_send_interface.hpp"
#include "mpi_receive_interface.hpp"

#define MAX_MESSAGE_SIZE (max(sizeof(MPIRequestMessage), sizeof(MPITokenMessage)))

class MPIConnection : public MPISendInterface, public MPIReceiveInterface {
private:
  void send_message(unsigned receiver, void *message, int message_size, int tag);

public:
  MPIConnection(int *argc, char ***argv);
  ~MPIConnection();

  void send_token(unsigned receiver, MPITokenMessage *message);
  void send_request(unsigned receiver, MPIRequestMessage *message);
  void receive_message(MPILock &resource_lock);

  unsigned get_index();
  unsigned get_size();
};

#endif

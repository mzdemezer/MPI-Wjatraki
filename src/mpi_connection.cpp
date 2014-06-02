#include "mpi_connection.hpp"

MPIConnection::MPIConnection(int *argc, char ***argv) {
  MPI_Init(argc, argv);
}

MPIConnection::~MPIConnection() {
  MPI_Finalize();
}

unsigned get_index() {
  unsigned index;
  MPI_Comm_rank(MPI_COMM_WORLD, &index);
  return index;
}

unsigned get_size() {
  unsigned size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  return size;
}

void MPIConnection::send_token(unsigned receiver, MPITokenMessage *message) {
  send_message(receiver, (void*)message, sizeof(MPITokenMessage), TOKEN_TAG);
}

void MPIConnection::send_request(unsigned receiver, MPIRequestMessage *message) {
  send_message(receiver, (void*)message, sizeof(MPIRequestMessage), REQUEST_TAG);
}

void MPIConnection::send_message(unsigned receiver, void *message, int message_size, int tag) {
  MPI_Send(message, message_size, MPI_BYTE, receiver, tag, MPI_COMM_WORLD);
}

void MPIConnection::receive_message(MPILock &resource_lock) {
  MPI_Status status;
  char buf[MAX_MESSAGE_SIZE];
  MPI_Recv(buf, MAX_MESSAGE_SIZE, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

  MPIMessage *message;
  switch (status.MPI_TAG) {
    case TOKEN_TAG:
      message = new MPITokenMessage(*((MPITokenMessage*)buf));
      break;
    case REQUEST_TAG:
      message = new MPIRequestMessage(*((MPIRequestMessage*)buf));
      break;
  }
  message->deliver(status.MPI_SOURCE, resource_lock);
  delete message;
}

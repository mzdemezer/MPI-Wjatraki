#include "mpi_connection.hpp"

void MPIConnection::send_token(unsigned receiver, MPITokenMessage *message) {
  send_message(receiver, (void*)message, sizeof(MPITokenMessage), TOKEN_TAG);
}

void MPIConnection::send_request(unsigned receiver, MPIRequestMessage *message) {
  send_message(receiver, (void*)message, sizeof(MPIRequestMessage), REQUEST_TAG);
}

void MPIConnection::send_message(unsigned receiver, void *message, int message_size, int tag) {
  MPI_Send(message, message_size, MPI_BYTE, receiver, tag, MPI_COMM_WORLD);
}

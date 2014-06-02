#include "mpi_controller.hpp"

MPIController::MPIController(int *argc, char ***argv, unsigned tree_rank)
: connection(argc, argv) {
  resource_lock = new MPILock(connection.get_index(), connection.get_size(), tree_rank, &connection);
}

MPIController::~MPIController() {
  delete resource_lock;
}

void MPIController::add_resource(Resource resource, unsigned tokens) {
  resource_lock->add_resource(resource, tokens);
}

void MPIController::handle_message() {
  connection.receive_message(*resource_lock);
}

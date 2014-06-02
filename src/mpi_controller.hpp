#ifndef _INCLUDE_MPI_CONTROLLER_HPP
#define _INCLUDE_MPI_CONTROLLER_HPP

class MPIController;

#include "common.hpp"
#include "mpi_connection.hpp"

class MPIController {
private:
  MPIConnection connection;
  MPILock *resource_lock;

public:
  MPIController(int *argc, char ***argv, unsigned tree_rank);
  ~MPIController();

  void add_resource(Resource resource, unsigned tokens);
  void handle_message();

  MonitorLock *get_lock() { return resource_lock; }
};

#endif

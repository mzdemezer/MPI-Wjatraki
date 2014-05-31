#include "common.hpp"
#include "mpi_lock.hpp"


int main(int argc, char **argv) {
  int size,                          // MPI Processes number
      rank;                          // MPI Process ID

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // while (working) {

    request_resource(HORSE);
    request_resource(WINDMILL);

    // fight

    release_resource(WINDMILL);
    release_resource(HORSE);

  // }

  // sync
  // clean

  MPI_Finalize();
  return 0;
}

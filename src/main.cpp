#include <mpi.h>
#include "common.hpp"
#include "mpi_lock.hpp"

int main(int argc, char **argv) {
   int size, rank;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   printf("SIZE = %d RANK = %d\n",size,rank);
   MPI_Finalize();
   return 0;
}

#include <mpi.h>
#include <queue>
#include <vector>
#include "common.hpp"
#include "mpi_lock.hpp"

int tokens_num[SIDES_NUM][RES_NUM],   // Number of available tokens in neighbour nodes and in self
    nodes_num[SIDES_NUM],             // Number of descedants in neighbour nodes and in self
    tokens[RES_NUM];                  // Number of of available tokens of each resource
   
unsigned short state[RES_NUM] = {     // Current state
  IDLE, IDLE
};                                    
queue<short> requests[RES_NUM];       // FIFO of nodes requesting token for certain resource

void request_token(short resource, vector<short> *sides);
void request_resource(short resource);
void release_resource(short resource);
void change_state(short state); 
short roulette(short resource, vector<short> *sides);

int main(int argc, char **argv) {
   int size,                          // MPI Processes number
       rank;                          // MPI Process ID

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   request_resource(WINDMILL);
   request_resource(HORSE);

   // fight
   
   release_resource(HORSE);
   release_resource(WINDMILL);
   
   MPI_Finalize();
   return 0;
}

void request_resource(short resource) {
  if (tokens_num[SELF] > 0)
    change_state(HAS_TOKEN);
  else {
    requests[resource].push(SELF);
    request_token(resource, new vector<short>{PARENT, LEFT, RIGHT});
  }
}

void change_state(short state) {}
void release_resource(short resource) {}
short roulette(short resource, vector<short> *sides) { return *(sides->begin()); }

void request_token(short resource, vector<short> *sides) {
  for(auto iter = sides->begin(); iter != sides->end(); ++iter)
    if(tokens_num[*iter][resource] <= 0)
      sides->erase(iter);

  if(!sides->empty()) {
    short side = roulette(resource, sides);
    tokens_num[side][resource] -= 1;
    // send_request
  }
}

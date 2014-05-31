#ifndef _INCLUDE_MPI_LOCK_HPP
#define _INCLUDE_MPI_LOCK_HPP

class MPILock;

#include "common.hpp"
#include "monitor_lock.hpp"

#define SIDE_INDEX_SELF 0
#define SIDE_INDEX_PARENT 1
#define ROOT_INDEX 0

typedef enum {
  IDLE = 0
, HAS_TOKEN
} MPILockState;

class MPILock : public MonitorLock {
  unsigned index, size, tree_rank;
  vector<int> sides;
  map<int, unsigned> side_map;
  vector<unsigned> no_tokens;                // Number of available tokens in neighbour nodes and in self
  unsigned sum_of_tokens;
  queue<unsigned> requests;                  // FIFO of nodes requesting token
  Resource resource;                         // type of resource

  MPILockState state;                        // Current state
  MPISendInterface *interface;

  void initialize_sides();
  void initialize_side_map();
  int parent_index();
  void initialize_tokens(unsigned tokens);
  unsigned get_side_index(unsigned sender);
  void change_state(MPILockState new_state);
  void try_request_token(vector<unsigned> &choices);
  void deliver_token();
  unsigned roulette(vector<short> &choices);
public:
  MPILock(unsigned index, unsigned size, unsigned tree_rank, unsigned tokens, Resource resource);
  void reserve();
  void release();

  void receive_token(unsigned sender, msg);
  void receive_request(unsigned sender);
};

#endif

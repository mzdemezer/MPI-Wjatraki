#ifndef _INCLUDE_MPI_RESOURCE_HPP
#define _INCLUDE_MPI_RESOURCE_HPP

#include "common.hpp"
#include "utilities/lockable.hpp"

#define SIDE_INDEX_SELF 0
#define SIDE_INDEX_PARENT 1
#define SIDE_INDEX_FIRST_CHILD 2
#define ROOT_INDEX 0

typedef enum {
  IDLE = 0
, HAS_TOKEN
} MPIState;

class MPIResource : public Lockable {
private:
  Resource type;
  vector<unsigned> no_tokens;    // Number of available tokens in neighbour nodes and in self
  unsigned sum_of_tokens;
  queue<unsigned> requests;      // FIFO of nodes requesting token - sides' indices
  MPIState state;                // Current state

public:
  MPIResource(Resource type, unsigned no_sides, unsigned tokens, bool is_root);

  void change_state(MPIState new_state);
  void push_request(unsigned side);
  unsigned pop_request();

  Resource get_type() { return type; }

  bool has_any_requests();

  void add_token(unsigned side);
  void remove_token(unsigned side);
  void transfer_token(unsigned from, unsigned to);
  bool has_any_tokens(unsigned side);
  bool empty_tokens();
  bool should_send_back();
  bool can_give_token();

  string queue_to_str(vector<int> &sides);
  string tokens_to_str(vector<int> &sides);
  void print_state(vector<int> &sides);
};

#endif

#ifndef _INCLUDE_MPI_RESOURCE_HPP
#define _INCLUDE_MPI_RESOURCE_HPP

#include "common.hpp"

#define SIDE_INDEX_SELF 0
#define SIDE_INDEX_PARENT 1
#define SIDE_INDEX_FIRST_CHILD 2
#define ROOT_INDEX 0

typedef enum {
  IDLE = 0
, HAS_TOKEN
} MPIState;

class MPIResource {
private:
  Resource type;
  vector<unsigned> no_tokens;    // Number of available tokens in neighbour nodes and in self
  unsigned sum_of_tokens;
  queue<unsigned> requests;      // FIFO of nodes requesting token - sides' indices
  MPIState state;                // Current state

public:
  MPIResource(Resource type, unsigned no_sides, unsigned tokens, bool is_root)
  : type(type)
  , sum_of_tokens(tokens)
  , state(IDLE) {
    no_tokens.reserve(no_sides);
    no_tokens.insert(no_tokens.begin(), no_sides, 0);
    if (is_root) {
      no_tokens[SIDE_INDEX_SELF] = tokens;
    } else {
      no_tokens[SIDE_INDEX_PARENT] = tokens;
    }
  }

  void change_state(MPIState new_state) { state = new_state; }

  void push_request(unsigned side) { requests.push(side); }
  unsigned pop_request() {
    unsigned side = requests.front();
    requests.pop();
    return side;
  }

  bool has_any_requests() { return !requests.empty(); }

  void add_token(unsigned side) {
    no_tokens[side] += 1;
    sum_of_tokens += 1;
  }
  void remove_token(unsigned side) {
    no_tokens[side] -= 1;
    sum_of_tokens -= 1;
  }
  void transfer_token(unsigned from, unsigned to) {
    no_tokens[from] -= 1;
    no_tokens[to]   += 1;
  }
  bool has_any_tokens(unsigned side) { return no_tokens[side] > 0; }
  unsigned get_no_tokens(unsigned side) { return no_tokens[side]; }
  unsigned get_sum_of_tokens() { return sum_of_tokens; }
  bool empty_tokens() { return sum_of_tokens == 0; }
  bool should_send_back() { return empty_tokens() && has_any_requests(); }
  bool can_give_token() {
    return no_tokens[SIDE_INDEX_SELF] > 1 ||
         ((no_tokens[SIDE_INDEX_SELF] == 1) && (state == IDLE));
  }

  Resource get_type() { return type; }
};

#endif

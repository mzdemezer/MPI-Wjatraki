#include "mpi_resource.hpp"

MPIResource::MPIResource(Resource type, unsigned no_sides, unsigned tokens, bool is_root)
: Lockable()
, type(type)
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

void MPIResource::change_state(MPIState new_state) {
  state = new_state;
  notify();
}

void MPIResource::push_request(unsigned side) {
  requests.push(side);
}

unsigned MPIResource::pop_request() {
  unsigned side = requests.front();
  requests.pop();
  return side;
}

bool MPIResource::has_any_requests() {
  return !requests.empty();
}

void MPIResource::add_token(unsigned side) {
  no_tokens[side] += 1;
  sum_of_tokens += 1;
}

void MPIResource::remove_token(unsigned side) {
  no_tokens[side] -= 1;
  sum_of_tokens -= 1;
}

void MPIResource::transfer_token(unsigned from, unsigned to) {
  no_tokens[from] -= 1;
  no_tokens[to]   += 1;
}

bool MPIResource::has_any_tokens(unsigned side) {
  return no_tokens[side] > 0;
}

bool MPIResource::empty_tokens() {
  return sum_of_tokens == 0;
}

bool MPIResource::should_send_back() {
  return empty_tokens() && has_any_requests();
}

bool MPIResource::can_give_token() {
  return no_tokens[SIDE_INDEX_SELF] > 1 ||
       ((no_tokens[SIDE_INDEX_SELF] == 1) && (state == IDLE));
}

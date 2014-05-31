#include "mpi_lock.hpp"

MPILock::MPILock(unsigned index, unsigned size, unsigned tree_rank, unsigned tokens,
  Resource resource, MPISendInterface *interface)
: index(index)
, size(size)
, tree_rank(tree_rank)
, sum_of_tokens(tokens)
, resource(resource)
, interace(interace) {
  initialize_sides();
  initialize_side_map();
  initialize_tokens(tokens);
}

void MPILock::initialize_sides() {
  sides.reserve(tree_rank + 2);
  sides.push_back(parent_index());
  sides.push_back(index);

  for (unsigned child_num = 1, child_index = tree_rank * index + child_num;
      (child_num <= tree_rank) && (child_index < size);
      (child_num += 1), (child_index += 1) ) {
    sides.push_back(child_index);
  }

}

void MPILock::initialize_side_map() {
  for (unsigned side_index = 0, len = sides.size(); side_index < len; side_index += 1) {
    int side = sides[side_index];
    side_map[side] = side_index;
  }
}

int MPILock::parent_index() {
  if (index == ROOT_INDEX) {
    return -1;
  } else {
    return (index - 1) / tree_rank;
  }
}

void MPILock::initialize_tokens(unsigned tokens) {
  no_tokens.reserve(sides.size());
  no_tokens.insert(no_tokens.begin(), sides.size(), 0);
  if (index == ROOT_INDEX) {
    no_tokens[SIDE_INDEX_SELF] = tokens;
  } else {
    no_tokens[SIDE_INDEX_PARENT] = tokens;
  }
}

void MPILock::reserve() {
  if (no_tokens[SIDE_INDEX_SELF] > 0) {
    change_state(HAS_TOKEN);
  } else {
    queue.push_back(SIDE_INDEX_SELF);
    // vector SIDE_INDEX_PARENT..sides.size()
    // try_request_token();
  }
}

void MPILock::release() {
  change_state(IDLE);
  deliver_token();
}


// sum_of_tokens

void MPILock::deliver_token() {
  if (!queue.empty()) {
    side_index = queue.front();
    queue.pop();

    if (side_index == SIDE_INDEX_SELF) {
      change_state(HAS_TOKEN);
    } else {
      no_tokens[SIDE_INDEX_SELF] -= 1;

      MPITokenMessage message((sum_of_tokens == 1) && !queue.empty());
      if (message.send_back) {
        sum_of_tokens -= 1;
      } else {
        no_tokens[side_index] += 1;
      }
      interace->send_token(sides[side_index], resource, message);
    }
  }
}

// when
void MPILock::receive_token(unsigned sender, msg) {
  no_tokens[SIDE_INDEX_SELF] += 1;
  sum_of_tokens += 1;

  if (message.send_back) {
    // assert in map??
    queue.push(get_side_index(sender));
  }
  deliver_token();
}

// when
void MPILock::receive_request(unsigned sender) {
  side_index = get_side_index(sender);
  if (no_tokens[SIDE_INDEX_SELF] > 1 ||
     ((no_tokens[SIDE_INDEX_SELF] == 1) && (state == IDLE)) ) {
    no_tokens[SIDE_INDEX_SELF] -= 1;
    no_tokens[side_index] += 1;
    MPITokenMessage message(false);
    interace->send_token(sides[side_index], resource, message);
  } else {
    queue.push_back(side_index);
    // [LEFT, RIGHT, PARENT] - [side]
    // try_request_token()
  }
}

unsigned MPILock::get_side_index(unsigned sender) {
  return side_map[(int)sender];
}

void MPILock::try_request_token(vector<unsigned> &choices) {
  if (sum_of_tokens == 0) { return; }

  for (unsigned i = 0; side_index < choices.size();) {
    if (no_tokens[choices[i]] == 0) {
      choices.erase(choices.begin() + i);
    } else {
      i += 1;
    }
  }

  {
    unsigned side_index = roulette(choices);
    no_tokens[side_index] -= 1;
    sum_of_tokens -= 1;
    interace->send_request(sides[side_index], resource);
  }
}


void MPILock::change_state(MPILockState new_state) {
  state = new_state;
  // notify()
}

unsigned MPILock::roulette(vector<short> &choices) { return *(choices->begin()); }

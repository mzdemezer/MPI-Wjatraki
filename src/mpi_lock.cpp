#include "mpi_lock.hpp"

MPILock::MPILock(unsigned index, unsigned size, unsigned tree_rank, MPISendInterface *interface)
: index(index)
, size(size)
, tree_rank(tree_rank)
, interface(interface) {
  initialize_sides();
  initialize_side_map();
}

MPILock::~MPILock() {
  for (map<Resource, MPIResource*>::iterator it = resources.begin(); it != resources.end(); ++it) {
    delete it->second;
  }

  resources.clear();
  side_map.clear();
}

void MPILock::initialize_sides() {
  sides.reserve(tree_rank + 2);
  sides.push_back(index);
  sides.push_back(index);
  sides[SIDE_INDEX_PARENT] = parent_index();

  for (unsigned child_num = 1, child_index = tree_rank * index + child_num;
      (child_num <= tree_rank) && (child_index < size);
      (child_num += 1), (child_index += 1) ) {
    sides.push_back(child_index);
  }
  if (DEBUG) {
    printf("#%u: sides(%lu) ", index, sides.size());
    printf("PARENT: % 3d,\t", sides[SIDE_INDEX_PARENT]);
    printf("SELF: % 3d", sides[SIDE_INDEX_SELF]);
    if (sides.size() > SIDE_INDEX_FIRST_CHILD) {
      printf(",\t{% 2d", sides[SIDE_INDEX_FIRST_CHILD]);
      for (unsigned i = SIDE_INDEX_FIRST_CHILD + 1; i < sides.size(); ++i) printf(", % 2d", sides[i]);
      printf(" }");
    }
    printf("\n");
  }
}

void MPILock::initialize_side_map() {
  for (unsigned side_index = 0, len = sides.size(); side_index < len; side_index += 1) {
    int side = sides[side_index];
    side_map[side] = side_index;
  }
}

int MPILock::parent_index() {
  if (is_root()) {
    return -1;
  } else {
    return (index - 1) / tree_rank;
  }
}

bool MPILock::is_root() {
  return index == ROOT_INDEX;
}

void MPILock::add_resource(Resource resource, unsigned tokens) {
  resources[resource] = new MPIResource(resource, sides.size(), tokens, is_root());
}

void MPILock::reserve(Resource resource) {
  MPIResource *res = get_resource(resource);

  res->lock();
  if (DEBUG) printf("#%u: reserve(%s)\n", index, RESOURCE(resource));
  if (DEBUG) res->print_state(sides);
  if (res->has_any_tokens(SIDE_INDEX_SELF)) {
    res->change_state(HAS_TOKEN);
  } else {
    res->push_request(SIDE_INDEX_SELF);

    vector<unsigned> choices(1, SIDE_INDEX_PARENT);
    for (unsigned side_index = SIDE_INDEX_FIRST_CHILD, len = sides.size(); side_index < len; side_index += 1) {
      choices.push_back(side_index);
    }
    try_request_token(res, choices);
    if (DEBUG) res->print_state(sides);
    if (DEBUG) printf("#%u: wait(%s)\n", index, RESOURCE(resource));
    res->wait();
    if (DEBUG) printf("#%u: woken up(%s)\n", index, RESOURCE(resource));
  }
  if (DEBUG) printf("#%u: reserved successfully(%s)\n", index, RESOURCE(resource));
  if (DEBUG) res->print_state(sides);
  res->unlock();
}

void MPILock::release(Resource resource) {
  MPIResource *res = get_resource(resource);
  res->lock();
  if (DEBUG) printf("#%u: release(%s)\n", index, RESOURCE(resource));
  if (DEBUG) res->print_state(sides);
  res->change_state(IDLE);
  deliver_token(res);
  if (DEBUG) res->print_state(sides);
  res->unlock();
}

void MPILock::deliver_token(MPIResource *resource) {
  if (resource->has_any_requests()) {
    unsigned side_index = resource->pop_request();

    if (side_index == SIDE_INDEX_SELF) {
      resource->change_state(HAS_TOKEN);
    } else {
      resource->remove_token(SIDE_INDEX_SELF);

      bool send_back = resource->should_send_back();
      MPITokenMessage message(resource->get_type(), send_back);
      if (!send_back) {
        resource->add_token(side_index);
      }
      interface->send_token(sides[side_index], &message);
    }
  }
}

void MPILock::receive_token(unsigned sender, MPITokenMessage &message) {
  MPIResource *resource = get_resource(message.resource);

  resource->lock();
  if (DEBUG) printf("#%u: received token from #%u for %s\n", index, sender, RESOURCE(message.resource));
  if (DEBUG) resource->print_state(sides);
  resource->add_token(SIDE_INDEX_SELF);

  if (message.send_back) {
    resource->push_request(get_side_index(sender));
  }
  deliver_token(resource);
  if (DEBUG) resource->print_state(sides);
  resource->unlock();
}

void MPILock::receive_request(unsigned sender, MPIRequestMessage &message) {
  MPIResource *resource = get_resource(message.resource);

  unsigned side_index = get_side_index(sender);

  resource->lock();
  if (DEBUG) printf("#%u: requested by #%u for %s\n", index, sender, RESOURCE(message.resource));
  if (DEBUG) resource->print_state(sides);
  if (resource->can_give_token()) {
    resource->transfer_token(SIDE_INDEX_SELF, side_index);
    MPITokenMessage message(resource->get_type(), false);
    interface->send_token(sides[side_index], &message);
  } else {
    resource->push_request(side_index);

    vector<unsigned> choices(1, SIDE_INDEX_PARENT);
    for (unsigned idx = SIDE_INDEX_FIRST_CHILD, len = sides.size(); idx < len; idx += 1) {
      if(idx != side_index) { choices.push_back(side_index); }

    if (DEBUG >= 2) {
      printf("#%u: choices(%lu) - { %u }", index, choices.size(), side_index);
      if (choices.size() > 0) {
        printf(",\t{ %u", choices[0]);
        for (unsigned i = 1; i < choices.size(); ++i) printf(", %u", choices[i]);
        printf(" }");
      }
      printf("\n");
    }
    try_request_token(resource, choices);
  }
  if (DEBUG) resource->print_state(sides);
  resource->unlock();
}

unsigned MPILock::get_side_index(unsigned side) {
  return side_map[(int)side];
}

void MPILock::try_request_token(MPIResource *resource, vector<unsigned> &choices) {
  for (unsigned i = 0; i < choices.size();) {
    if (resource->has_any_tokens(choices[i])) {
      i += 1;
    } else {
      choices.erase(choices.begin() + i);
    }
  }

  if (choices.empty()) { return; }

  {
    unsigned side_index = roulette(choices);
    resource->remove_token(side_index);
    MPIRequestMessage message(resource->get_type());
    interface->send_request(sides[side_index], &message);
  }
}

unsigned MPILock::roulette(vector<unsigned> &choices) { return choices[0]; }

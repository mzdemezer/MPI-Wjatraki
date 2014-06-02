#ifndef _INCLUDE_MPI_LOCK_HPP
#define _INCLUDE_MPI_LOCK_HPP

class MPILock;

#include "common.hpp"
#include "utilities/lockable.hpp"
#include "mpi_resource.hpp"
#include "mpi_send_interface.hpp"
#include "monitor_lock.hpp"
#include "messages/mpi_token_message.hpp"
#include "messages/mpi_request_message.hpp"

class MPILock : public MonitorLock, protected Lockable {
  unsigned index, size, tree_rank;
  vector<int> sides;
  map<int, unsigned> side_map;

  map<Resource, MPIResource*> resources;

  MPISendInterface *interface;

  void initialize_sides();
  void initialize_side_map();
  int parent_index();
  unsigned get_side_index(unsigned side);
  void change_state(MPIResource *resource, MPIState new_state);
  void try_request_token(MPIResource *resource, vector<unsigned> &choices);
  void deliver_token(MPIResource *resource);
  unsigned roulette(vector<unsigned> &choices);

  MPIResource *get_resource(Resource resource) {
    return resources[resource];
  }
public:
  MPILock(unsigned index, unsigned size, unsigned tree_rank, MPISendInterface *interface);
  ~MPILock();

  void add_resource(Resource resource, unsigned tokens);
  bool is_root();

  void reserve(Resource resource);
  void release(Resource resource);

  void receive_token(unsigned sender, MPITokenMessage &message);
  void receive_request(unsigned sender, MPIRequestMessage &message);

  unsigned get_index() { return index; }
};

#endif

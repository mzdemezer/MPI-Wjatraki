#ifndef _INCLUDE_MPI_MESSAGE_HPP
#define _INCLUDE_MPI_MESSAGE_HPP

class MPIMessage;
class MPILock;

class MPIMessage {
public:
  virtual void deliver(unsigned sender, MPILock &resource_lock) = 0;
  virtual ~MPIMessage() {}
};

#endif

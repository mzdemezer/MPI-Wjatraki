CXX=mpic++
CXX_FLAGS=-Wall -g -std=c++11

TARGET=wjatraki

UTILITIES=lockable
SUBTARGET_UTILITES=$(addprefix utilities/, $(UTILITIES))

MESSAGES_I=mpi_message
MESSAGES_S=mpi_token_message mpi_request_message
INTERFACE_MESSAGES=$(addprefix messages/, $(MESSAGES_I))
SUBTARGET_MESSAGES=$(addprefix messages/, $(MESSAGES_S))

INTERFACES=monitor_lock mpi_send_interface mpi_receive_interface $(INTERFACE_MESSAGES)
SUBTARGETS=$(SUBTARGET_UTILITES) $(SUBTARGET_MESSAGES) common mpi_resource mpi_lock mpi_controller mpi_connection

TARGET_FILES=$(SUBTARGETS) main
HEADER_FILES=$(SUBTARGETS) $(INTERFACES)

SOURCES=$(addprefix src/, $(addsuffix .cpp, $(TARGET_FILES)))
HEADERS=$(addprefix src/, $(addsuffix .hpp, $(HEADER_FILES)))
OBJS=$(addprefix obj/, $(addsuffix .o, $(TARGET_FILES)))

all: init $(TARGET)

init:
	-mkdir -p obj
	-mkdir -p obj/utilities
	-mkdir -p obj/messages

$(TARGET): $(OBJS)
	$(CXX) $(CXX_FLAGS) $(LIBS) -o $@ $(OBJS)

obj/%.o: src/%.cpp $(HEADERS)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	-rm -rf obj $(TARGET)

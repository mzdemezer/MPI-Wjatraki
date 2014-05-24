CXX=mpic++
CXX_FLAGS=-Wall -g -std=c++11

TARGET=wjatraki

INTERFACES=common

SUBTARGETS=mpi_lock

TARGET_FILES=$(SUBTARGETS) main
HEADER_FILES=$(SUBTARGETS) $(INTERFACES)

SOURCES=$(addprefix src/, $(addsuffix .cpp, $(TARGET_FILES)))
HEADERS=$(addprefix src/, $(addsuffix .hpp, $(HEADER_FILES)))
OBJS=$(addprefix obj/, $(addsuffix .o, $(TARGET_FILES)))

all: init $(TARGET)

init:
	-mkdir -p obj

$(TARGET): $(OBJS)
	$(CXX) $(CXX_FLAGS) $(LIBS) -o $@ $(OBJS)

obj/%.o: src/%.cpp $(HEADERS)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	-rm -rf obj $(TARGET)

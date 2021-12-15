CC = g++
BINARIES = $(patsubst src/%.cpp,bin/%.o, $(wildcard src/*.cpp)) $(patsubst ../common/src/%.cpp,bin/%.o, $(wildcard ../common/src/*.cpp)) 
# Remove -g if not in debug mode
LIBS_FLAGS = -I../common/include -lstdc++fs -pthread -lncurses -lboost_filesystem -lboost_system 
COMPILE_FLAGS = -Wall -g -Werror -I./include -std=c++17

.PHONY: all test clean
all: main

main: $(BINARIES)
	$(CC) $^ -o $@ $(LIBS_FLAGS) 

bin/%.o: src/%.cpp
	mkdir -p bin
	$(CC) -c -o $@ $(LIBS_FLAGS) $(COMPILE_FLAGS) $^

bin/%.o: ../common/src/%.cpp
	mkdir -p bin
	$(CC) -c -o $@ $(LIBS_FLAGS) $(COMPILE_FLAGS) $^

clean:
	rm -rf bin/ main

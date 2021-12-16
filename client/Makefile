CC = g++

TARGET = client.out

OUT_DIR = bin/
BINARIES = $(patsubst src/%.cpp,${OUT_DIR}%.o, $(wildcard src/*.cpp)) $(patsubst ../common/src/%.cpp,${OUT_DIR}%.o, $(wildcard ../common/src/*.cpp)) 
# Remove -g if not in debug mode
LIBS_FLAGS = -I../common/include -lstdc++fs -pthread -lncurses -lboost_filesystem -lboost_system -lboost_serialization
COMPILE_FLAGS = -Wall -g -Werror -I./include -std=c++17

MKDIR_P = mkdir -p

all: directory ../${TARGET}

directory: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

../${TARGET}: $(BINARIES)
	$(CC) $^ -o $@ $(LIBS_FLAGS) 

${OUT_DIR}%.o: src/%.cpp
	$(CC) -c -o $@ $(LIBS_FLAGS) $(COMPILE_FLAGS) $^

${OUT_DIR}%.o: ../common/src/%.cpp
	$(CC) -c -o $@ $(LIBS_FLAGS) $(COMPILE_FLAGS) $^

clean:
	rm -rf ${OUT_DIR} main

.PHONY: all test clean directory
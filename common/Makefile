CC = g++
FLAGS = -Wall -g
OBJ = date.o error.o

all: $(OBJ)

date.o: date.hpp date.cpp
	$(CC) $(FLAGS) -c date.cpp
error.o: error.hpp error.cpp
	$(CC) $(FLAGS) -c error.cpp
.PHONY: clean
clean:
	@rm -f $(OBJ) *~

CC = g++
FLAGS = -Wall -g
OBJ = userInfo.o file.o relation.o message.o

userInfo.o: ../common/userData.hpp userInfo.hpp userInfo.cpp ../common/error.hpp file.hpp relation.hpp btree.hpp message.hpp
	$(CC) $(FLAGS) -c userInfo.cpp
file.o: file.cpp file.hpp
	$(CC) $(FLAGS) -c $<
message.o: message.cpp message.hpp ../common/userData.hpp file.hpp ../common/error.hpp
	$(CC) $(FLAGS) -c $<
relation.o: relation.cpp relation.hpp file.hpp ../common/userData.hpp ../common/error.hpp btree.hpp
	$(CC) $(FLAGS) -c $<
error.o: ../common/error.hpp ../common/error.cpp
	(cd ../common; make error.o)
	cp ../common/error.o .
btreeTester.o: btreeTester.cpp btree.hpp
	$(CC) $(FLAGS) -c $<
btreeTester: btreeTester.o error.o file.o
	$(CC) $(FLAGS) -o $@ $^
test: $(OBJ) test.o error.o 
	$(CC) $(FLAGS) -o $@ $^
test.o: test.cpp userInfo.hpp
	$(CC) $(FLAGS) -c $<

.PHONY: clean
clean:
	@rm -f *~ $(OBJ) test.o btreeTester.o error.o *.dat test btreeTester

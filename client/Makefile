OBJ = client.o user.o date.o error.o userInfo.o file.o relation.o
CC = g++
FLAGS = -Wall -g

client: $(OBJ)
	$(CC) $(FLAGS) -o client $(OBJ)
client.o: client.cpp user.hpp
	$(CC) $(FLAGS) -c client.cpp
user.o: user.hpp user.cpp ../common/date.hpp ../common/userData.hpp ../background/userInfo.hpp
	$(CC) $(FLAGS) -c user.cpp
error.o: ../common/error.hpp ../common/error.cpp
	(cd ../common; make error.o)
	cp ../common/error.o .
date.o: ../common/date.hpp ../common/date.cpp
	(cd ../common; make date.o)
	cp ../common/date.o .
userInfo.o: ../background/userInfo.hpp ../background/userInfo.cpp
	(cd ../background; make userInfo.o)
	cp ../background/userInfo.o .
relation.o: ../background/relation.hpp ../background/relation.cpp
	(cd ../background; make relation.o)
	cp ../background/relation.o .
file.o: ../background/file.hpp ../background/file.cpp
	(cd ../background; make file.o)
	cp ../background/file.o .
tidy:
	@rm -f *~ $(OBJ) *.dat
.PHONY: clean
clean:
	@rm -f *~ $(OBJ) client *.dat
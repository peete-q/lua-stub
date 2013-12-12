
CC = g++
CFLAGS = -g -I../include
LFLAGS = -L../lib -llua
T = test.exe
OBJ = test.o

all : $(OBJ)
	$(CC) -o $(T) $(OBJ) $(LFLAGS)

test.o : test.cpp
	$(CC) -c test.cpp $(CFLAGS)

clean :
	rm -f $(T) $(OBJ)

CC=g++
LD=g++
CFLAGS=-Wall -O2 -std=c++0x -ggdb
LDFLAGS=
RM=rm
OBJECTS=main.o matriz.o

.PHONY: all clean

all: tp1

clean:
	reset
	- $(RM) -rf *.o *~ tp1

tp1: $(OBJECTS)
	$(LD) $(LDFLAGS) -o ../bin/tp1 $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

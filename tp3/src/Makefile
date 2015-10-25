#------- Commands ------- 
CC=g++
LD=g++
RM=rm

#------- Flags ------- 
LDFLAGS=

#input from zero mode off
CFLAGS=-Wall -ggdb -O2 -std=c++11 -I../include/ 

#------- Objects ------- 

OBJS=main.o io.o slowmotion.o
#------- Dirs ------- 

BINDIR=../bin
TESTS=tests

.PHONY: all clean clean_test_results

all: tp3

clean:
	reset
	rm -rf $(BINDIR)/*
	rm -rf *.o *~ tp3

tp3: $(OBJS)
	mkdir -p $(BINDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp3 $(OBJS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"
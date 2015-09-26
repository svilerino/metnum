CC=g++
LD=g++
#vectorize mode off
CFLAGS=-Wall -O2 -std=c++11 -I../include/ -D_DOK_SPARSE_VECTOR
#vectorize mode on
#CFLAGS=-Wall -O2 -ftree-vectorize -ftree-vectorizer-verbose=1 -mavx -std=c++11 -I../include/ -D_DOK_SPARSE_VECTOR
LDFLAGS=
RM=rm
OBJECTS=main.o matriz.o io.o problem.o
BINDIR=../bin
TESTTIMINGOUTDIR=timings-out
TESTS=tests

.PHONY: all clean clean_tests

all: tp2

clean:
	reset
	rm -rf $(BINDIR)	
	rm -rf *.o *~ tp2

clean_tests:
	reset
	rm -rf $(TESTTIMINGOUTDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	rm -rf timing_results.txt
	rm -rf $(TESTS)/*.out

tp2: $(OBJECTS)
	mkdir -p $(BINDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp2 $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

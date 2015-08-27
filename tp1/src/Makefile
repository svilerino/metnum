CC=g++
LD=g++
CFLAGS=-Wall -O2 -std=c++0x -ggdb
LDFLAGS=
RM=rm
OBJECTS=main.o matriz.o io.o problem.o
BINDIR=../bin
TESTTIMINGOUTDIR=timings-out

.PHONY: all clean clean_tests

all: tp1

run_gauss: tp1
	./run_performance_tests.sh eliminacion_gaussiana
	./run_correctitude_tests.sh eliminacion_gaussiana
	

run_factorizacion_lu: tp1
	./run_performance_tests.sh factorizacion_lu
	./run_correctitude_tests.sh factorizacion_lu


run_gauss_pivot: tp1
	./run_performance_tests.sh eliminacion_gaussiana_pivoteo_parcial
	./run_correctitude_tests.sh eliminacion_gaussiana_pivoteo_parcial

clean:
	reset
	rm -rf $(BINDIR)	
	- $(RM) -rf *.o *~ tp1

clean_tests:
	reset
	rm -rf $(TESTTIMINGOUTDIR)
	mkdir -p $(TESTTIMINGOUTDIR)

tp1: $(OBJECTS)
	mkdir -p $(BINDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp1 $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

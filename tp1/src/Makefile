CC=g++
LD=g++
CFLAGS=-Wall -O2 -std=c++0x -ggdb
LDFLAGS=
RM=rm
OBJECTS=main.o matriz.o io.o problem.o
BINDIR=../bin
TESTTIMINGOUTDIR=timings-out
TESTS=tests

.PHONY: all clean clean_tests

all: tp1

run_gauss: tp1
	./run_performance_tests.sh eliminacion_gaussiana
	./run_correctness_tests.sh eliminacion_gaussiana
	

run_factorizacion_lu: tp1
	./run_performance_tests.sh factorizacion_lu
	./run_correctness_tests.sh factorizacion_lu


run_gauss_pivot: tp1
	./run_performance_tests.sh eliminacion_gaussiana_pivoteo_parcial
	./run_correctness_tests.sh eliminacion_gaussiana_pivoteo_parcial

clean:
	reset
	rm -rf $(BINDIR)	
	- $(RM) -rf *.o *~ tp1

clean_tests:
	reset
	rm -rf $(TESTTIMINGOUTDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	rm -rf $(TESTS)/*.png
	rm -rf $(TESTS)/*.out
	rm -rf $(TESTS)/*.isoout

tp1: $(OBJECTS)
	mkdir -p $(BINDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp1 $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

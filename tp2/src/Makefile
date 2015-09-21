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

all: tp2

clean:
	reset
	rm -rf $(BINDIR)	
	- $(RM) -rf *.o *~ tp2

clean_tests:
	reset
	rm -rf $(TESTTIMINGOUTDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	rm -rf isofile_result.txt
	rm -rf timing_results.txt
	rm -rf seguridad_isoterma_result.txt
	rm -rf $(TESTS)/*.png
	rm -rf $(TESTS)/*.out
	rm -rf $(TESTS)/*.isoout
	rm -rf $(TESTS)/*.seguridadisoout
	rm -rf $(TESTS)/*.mp4

tp2: $(OBJECTS)
	mkdir -p $(BINDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp2 $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

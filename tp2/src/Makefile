#------- Commands ------- 
CC=g++
LD=g++
RM=rm

#------- Flags ------- 
LDFLAGS=

#input from zero mode off
CFLAGS=-Wall -ggdb -O2 -std=c++11 -I../include/ 

#input from zero mode on
#CFLAGS=-Wall -ggdb -O2 -std=c++11 -I../include/ -D_INPUT_FROM_ZERO

#------- Objects per target ------- 

TP2_OBJECTS=main.o io.o problem.o
TEST_DOK_OBJECTS= test_dok.o
TEST_CSR_OBJECTS= test_csr.o
TEST_POWMETHOD_OBJECTS= test_power_method.o
#------- Dirs ------- 

BINDIR=../bin
TESTS=tests

.PHONY: all clean clean_test_results

#all: tp2 csr_tests dok_tests power_method_tests
all: tp2

clean:
	reset
	rm -rf $(BINDIR)/*
	rm -rf *.o *~ tp2

clean_test_results:
	reset
	rm -rf $(TESTS)/*.out
	rm -rf $(TESTS)/*.png
	rm -rf $(TESTS)/*.convergence
	rm -rf $(TESTS)/*.timing

tp2: $(TP2_OBJECTS)
	mkdir -p $(BINDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp2 $(TP2_OBJECTS)

csr_tests: $(TEST_CSR_OBJECTS)
	$(LD) $(LDFLAGS) -o $(BINDIR)/csr_tests $(TEST_CSR_OBJECTS)

dok_tests: $(TEST_DOK_OBJECTS)
	$(LD) $(LDFLAGS) -o $(BINDIR)/dok_tests $(TEST_DOK_OBJECTS)

power_method_tests: $(TEST_POWMETHOD_OBJECTS)
	$(LD) $(LDFLAGS) -o $(BINDIR)/power_method_tests $(TEST_POWMETHOD_OBJECTS)

run_power_method_tests: tp2 power_method_tests
	valgrind $(BINDIR)/power_method_tests

run_csr_tests: tp2 csr_tests
	valgrind $(BINDIR)/csr_tests

run_dok_tests: tp2 dok_tests
	valgrind $(BINDIR)/dok_tests

run_pagerank_tests: tp2
	scripts/run_pagerank_tests.sh

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

#------- Commands ------- 
CC=g++
LD=g++
RM=rm

#------- Flags ------- 
LDFLAGS=

#vectorize mode off
CFLAGS=-Wall -ggdb -O2 -std=c++11 -I../include/

#vectorize mode on
#CFLAGS=-Wall -ggdb -O2 -ftree-vectorize -ftree-vectorizer-verbose=1 -mavx -std=c++11 -I../include/

#------- Objects per target ------- 

TP2_OBJECTS=main.o io.o problem.o
TEST_DOK_OBJECTS= test_dok.o
TEST_CSR_OBJECTS= test_csr.o
TEST_POWMETHOD_OBJECTS= test_power_method.o
#------- Dirs ------- 

BINDIR=../bin
TESTTIMINGOUTDIR=timings-out
TESTS=tests

.PHONY: all clean clean_test_results

all: tp2 csr_tests dok_tests power_method_tests

clean:
	reset
	rm -rf $(BINDIR)	
	rm -rf *.o *~ tp2

clean_test_results:
	reset
	rm -rf $(TESTTIMINGOUTDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	rm -rf timing_results.txt
	rm -rf $(TESTS)/*.out

tp2: $(TP2_OBJECTS)
	mkdir -p $(BINDIR)
	mkdir -p $(TESTTIMINGOUTDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp2 $(TP2_OBJECTS)

csr_tests: $(TEST_CSR_OBJECTS)
	$(LD) $(LDFLAGS) -o $(BINDIR)/csr_tests $(TEST_CSR_OBJECTS)

dok_tests: $(TEST_DOK_OBJECTS)
	$(LD) $(LDFLAGS) -o $(BINDIR)/dok_tests $(TEST_DOK_OBJECTS)

power_method_tests: $(TEST_POWMETHOD_OBJECTS)
	$(LD) $(LDFLAGS) -o $(BINDIR)/power_method_tests $(TEST_POWMETHOD_OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

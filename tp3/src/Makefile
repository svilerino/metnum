#------- Commands ------- 
CC=g++
LD=g++
RM=rm
OPENCV_LIB = `pkg-config --cflags --libs opencv`

#------- Flags ------- 
LDFLAGS=

#input from zero mode off
CFLAGS=-Wall -ggdb -O2 -std=c++11 -I../include/ 

#CFLAGS=-Wall -ggdb -O2 -std=c++11 -I../include/ -DCOLOR_PROCESSING

#------- Objects ------- 

OBJS=main.o slowmotion.o opencv_util.o interpolation.o matriz.o frame_analysis.o
OBJSVID2BN=videotograyscale.o opencv_util.o 
#------- Dirs ------- 

BINDIR=../bin
TESTS=tests

.PHONY: all clean clean_test_results

all: tp3 vid2grayscale

clean:
	reset
	rm -rf $(BINDIR)/*
	rm -rf *.o *~ tp3

tp3: $(OBJS)
	mkdir -p $(BINDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp3 $(OBJS) $(OPENCV_LIB) -lm

vid2grayscale: $(OBJSVID2BN)
	mkdir -p $(BINDIR)
	$(LD) $(LDFLAGS) -o $(BINDIR)/vid2grayscale $(OBJSVID2BN) $(OPENCV_LIB) -lm

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<" $(OPENCV_LIB)
#------- Commands ------- 
CC=g++
LD=g++
RM=rm
OPENCV_LIB = `pkg-config --cflags --libs opencv`

#------- Flags ------- 
LDFLAGS=

CFLAGS=-Wall -ggdb -O2 -std=c++11 -I../include/ -DAUTO_DROP_FRAMES

#flags opcionales en CFLAGS
#-DCOLOR_PROCESSING procesa los videos en color
#-DAUTO_DROP_FRAMES skipea los frames automaticamente para experimentacion

#------- Objects ------- 

OBJS=main.o slowmotion.o opencv_util.o interpolation.o matriz.o frame_analysis.o
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
	$(LD) $(LDFLAGS) -o $(BINDIR)/tp3 $(OBJS) $(OPENCV_LIB) -lm

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<" $(OPENCV_LIB)
CC = g++
CFLAGS = -ggdb -Wall -O2 -std=c++11 -pedantic -I .
OPENCV_LIB = `pkg-config --cflags --libs opencv`

BIN = tp3

OBJS = main.o io.o slowmotion.o opencv_util.o

.PHONY: all clean

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(OPENCV_LIB) -lm

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(OPENCV_LIB)

all: $(BIN)

clean:
	reset
	rm -f $(OBJS)
	rm -f ./$(BIN)
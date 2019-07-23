GXX = g++
FLAGS = -Wall -Wextra -O
TARGET = textex
SRC = *.cc
LIBS = -lstdc++fs
DFLAGS = -fsanitize=address -fsanitize=leak

all:
	$(GXX) $(FLAGS) $(SRC) -o $(TARGET) $(LIBS)


.PHONY:
	textex clean


debug:
	$(GXX) $(FLAGS) $(SRC) $(FLAGS) $(DFLAGS) -o $(TARGET) $(LIBS)


clean:
	rm textex


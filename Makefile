OBJS = source/mine.cpp

#Change compiler here if required
CC = g++

OBJ_NAME = mine

all : release

release:
	$(CC) $(OBJS) -std=c++11 -O3 -o $(OBJ_NAME)

debug:
	$(CC) $(OBJS) -std=c++11 -O0 -o $(OBJ_NAME)
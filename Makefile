OBJS = source/mine.cpp

#Change compiler here if required
CC = g++

OBJ_NAME = mine

all :
	$(CC) $(OBJS) -std=c++11 -o $(OBJ_NAME)

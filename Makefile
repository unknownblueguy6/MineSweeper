OBJS = source/mine.cpp

#Change compiler here if required
CC = g++

OBJ_NAME = MineSweeper

all :
	$(CC) $(OBJS) -o $(OBJ_NAME)
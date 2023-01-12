CC ?= gcc
TARGET ?= test


SRC = $(wildcard ../$(pwd)*.c)

OBJ = $(patsubst %.c, %.o, $(SRC))


all:
	$(CC) $(SRC) -o $(OBJ) 

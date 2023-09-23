.PHONY:clean
CC=gcc -no-pie
RM=rm
EXE=data_test
SRCS=$(wildcard *.c)
GDB = -g
# 把.c替换为.o
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(EXE):$(OBJS)
	$(CC) $(GDB) -o $@ $^
%.o:%.c
	$(CC) $(GDB) -o  $@ -c $^
clean:
	$(RM)  $(OBJS)
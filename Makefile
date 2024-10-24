OBJS	= proc.o functions.o
SOURCE	= proc.c functions.c
HEADER	= functions.h
OUT	= a.proc
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

server.o: server.c
	$(CC) $(FLAGS) proc.c 

functions.o: functions.c
	$(CC) $(FLAGS) functions.c 

clean:
	rm -f $(OBJS) $(OUT)
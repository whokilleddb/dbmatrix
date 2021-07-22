CC=gcc
CFLAGS=-g -Wall -Werror -O3 
OBJS=src/main.o

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lncurses -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf src/*.o src/main

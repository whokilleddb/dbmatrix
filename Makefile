CC=gcc
CFLAGS=-g -Wall -Werror -O3 
OBJS=src/main.o src/matrix.o src/ui.o 

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lncurses

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf src/*.o src/main

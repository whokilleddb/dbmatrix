# Compiler : GCC 
CC = gcc

CFLAGS = -lncurses -Wall -Werror -g -O3 -lm 

# Build DIR 
DIR = src
# Build Target Executable
TARGET = dbmatrix

all: $(TARGET)

$(TARGET): $(DIR)/$(TARGET).c
	$(CC)  $(DIR)/$(TARGET).c $(CFLAGS) -o $(TARGET)

clean:
	$(RM) $(TARGET)


# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -DDEBUG

# Source and object files
SRC = grid.c sudoku.c
OBJ = $(SRC:.c=.o)

# Executable name
BIN = sudoku

# Default target
all: $(BIN)

# Linking objects to produce the executable
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

# Compile each .c into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f *.o $(BIN)

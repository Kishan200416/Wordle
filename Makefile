# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Location of the source files
SRC = src/wordle.c

# Name of the final executable program
TARGET = wordle

# The default command that runs when you type "make"
all: $(TARGET)

# The rule for building the program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# A rule to clean up the compiled file
clean:
	rm -f $(TARGET)

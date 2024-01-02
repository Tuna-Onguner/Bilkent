#
# Bilkent University
# Semester of Fall 2023
#
# Department of Computer Engineering
#
# CS342 Operating Systems
# Project #3: Virtual Memory and Paging
#
# Section 1
# Instructor: İbrahim Körpeoğlu
#
# Authors: Deniz Tuna Onguner (22001788) and Alper Göçmen (22002948)
# Date: 4 December 2023, Monday
#

# Compiler
CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = #-lrt -lm

# List of source files
SRCS = memsim.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = memsim

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f $(OBJS)

# Compile
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

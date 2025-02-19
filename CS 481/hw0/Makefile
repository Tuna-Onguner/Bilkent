#
# Bilkent University
# Semester of Spring 2025
#
# Department of Computer Engineering
#
# CS481 Bioinformatics Algorithms
# Homework Assignment #0
#
# Section 1
# Instructor: Can Alkan
#
# Author: Deniz Tuna ONGUNER (22001788)
#
# Date: 15.02.2025 (DD/MM/YYYY)
# Description: A Makefile for compiling and running the program.
#
# Notes:
# - To compile the program, run 'make COMPILE', or 'make' for short.
# - To run the program, run 'make RUN ARGS="-n N -s S"', set N and S as desired.
# - To clean the directory, run 'make CLEAN'.
# - To compile, run, and clean the directory, run 'make COMPILE RUN ARGS="-n N -s S" CLEAN'.
#

# Compiler
CC = gcc

# Compiler flags:
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# -O1: Optimize for speed
# -std=gnu99: Use the GNU99 standard
CFLAGS = -Wall -Wextra -O1 -std=gnu99

# Target executable
TARGET = hw0

# Compile the program
COMPILE: $(TARGET)

$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Run the program
RUN: $(TARGET)
	./$(TARGET) $(ARGS)

# Clean the directory
CLEAN:
	rm -f $(TARGET) main.o

# End of Makefile

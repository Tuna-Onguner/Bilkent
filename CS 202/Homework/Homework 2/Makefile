# Title: Binary Search Trees
# Author: Deniz Tuna Onguner
# ID: 22001788
# Section: 2
# Homework: 2
# Description: Makefile; creates an executable file named hw2

CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=BSTNode.cpp BST.cpp main.cpp analysis.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hw2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

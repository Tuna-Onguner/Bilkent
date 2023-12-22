# Title: Heaps and AVL Tree
# Author: Deniz Tuna ONGUNER
# ID: 22001788
# Section: 1
# Assignment: 3
# Description: Makefile

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra


# Source files
SRCS := heap.cpp heapsort.cpp


# Object files
OBJS := $(SRCS:.cpp=.o)


# Executable
TARGET := heapsort


# Rules
.PHONY: all clean


# Targets
all: $(TARGET)


# Dependencies
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@


# Pattern rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Clean
clean:
	rm -f $(OBJS) $(TARGET)

# Title: Balanced Search Trees
# Author: Deniz Tuna ONGUNER
# ID: 22001788
# Section: 1
# Assignment: 4
# Description: Makefile



# Compiler
CXX := g++


# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -g


# Source files
SRCS := AVLTree.cpp main.cpp


# Object files
OBJS := $(SRCS:.cpp=.o)


# Executable
TARGET := avlfreq


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
	rm -f $(OBJS) $(TARGET) $(OTHER)

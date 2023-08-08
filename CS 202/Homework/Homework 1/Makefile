# Title: Sorting and Algorithm Efficiency
# Author: Deniz Tuna ONGUNER
# ID: 22001788
# Section: 1
# Homework: 1
# Description: Makefile

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++98 -Wall -Wextra


# Source files
SRCS := sorting.cpp main.cpp


# Object files
OBJS := $(SRCS:.cpp=.o)


# Executable
TARGET := hw1


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

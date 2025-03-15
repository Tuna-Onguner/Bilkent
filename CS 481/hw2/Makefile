# Detect Python command
PYTHON := $(shell command -v python3.12 || command -v python3.11 || command -v python3.10)
MAIN_SCRIPT := main.py
ARGS :=

# Default target
.PHONY: ALL
ALL: RUN

# "Compile" target (Python is not compiled)
.PHONY: COMPILE
COMPILE:
	@echo "Python is not a compiled language. Run 'make RUN ARGS=' instead."

# Run the main script with arguments
.PHONY: RUN
RUN:
	$(PYTHON) $(MAIN_SCRIPT) $(ARGS)

# Clean up cache files
.PHONY: CLEAN
CLEAN:
	rm -rf __pycache__ *.pyc *.pyo

# Help message
.PHONY: HELP
HELP:
	@echo "Makefile for Spring 2025 CS481 - Bioinformatics Algorithms Homework Assignment #2"
	@echo "Usage:"
	@echo "  make COMPILE                 - Show a message about Python not being compiled"
	@echo "  make RUN ARGS='arguments'    - Run the script with given arguments"
	@echo "  make CLEAN                   - Remove cache files"
	@echo "  make HELP                    - Show this help message"

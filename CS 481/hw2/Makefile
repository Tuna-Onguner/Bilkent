# Detect Python command
PYTHON := $(shell command -v python3.12 || command -v python3.11 || command -v python3.10)
MAIN_SCRIPT := main.py
ARGS :=

# Default target
.PHONY: ALL
ALL: RUN

# "Compile" target (Python is not compiled)
.PHONY: COMPILE
COMPILE:  # This command is included as it is required by the assignment
	@echo "Python is not a compiled language. Directly, run 'make RUN ARGS=' command instead. Run 'make HELP' for more information."

# Run the main script with arguments
.PHONY: RUN
RUN:
# Check if the minimum required Python version is installed
	@if [ -z "$(PYTHON)" ]; then \
		echo "Error: No compatible Python interpreter found. Please install Python 3.10 or later."; \
		exit 1; \
	fi
	$(PYTHON) $(MAIN_SCRIPT) $(ARGS)

# Clean cache files
.PHONY: CLEAN
CLEAN:
	rm -rf __pycache__ *.pyc *.pyo

# Clean all cache files and output files
.PHONY: CLEANALL
CLEANALL: CLEAN
# Remove all .txt files except README.txt
	find . -maxdepth 1 -type f -name "*.txt" ! -name "README.txt" -exec rm -f {} +

# Help message
.PHONY: HELP
HELP:
	@echo "Makefile for Spring 2025 CS481 - Bioinformatics Algorithms Homework Assignment #2"
	@echo "Usage:"
	@echo "  make COMPILE                 - Show a message about Python not being compiled"
	@echo "  make RUN ARGS='arguments'    - Run the script with given arguments"
	@echo "  make CLEAN                   - Remove cache files"
	@echo "  make CLEANALL                - Remove all cache files and output files, i.e. '.txt' files except README.txt"
	@echo "  make HELP                    - Show this help message"

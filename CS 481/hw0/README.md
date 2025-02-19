# CS481 Bioinformatics Algorithms - Homework Assignment #0

**Author:** Deniz Tuna Onguner  
**Bilkent ID:** 22001788  
**Semester:** Spring 2025  
**Date:** 15.02.2025  (DD.MM.YYYY)

## Overview

This program is written in C (using the GNU99 standard) and is designed to generate random square matrices. It performs the following operations on these matrices:
- **Matrix Addition:** Adds two square matrices.
- **Scalar Multiplication:** Multiplies a square matrix by a scalar value.
- **Matrix Transposition:** Computes the transpose of a square matrix.

## Files

- `main.c` — The main source code file for the project.

## Command-Line Arguments

The program requires the following command-line arguments:

- **`-n` (non-negative integer):**  
  Specifies the size of the square matrices to be generated.

- **`-s` (integer):**  
  Specifies the scalar value to be used for scalar multiplication.

## How to Build and Run

A Makefile is provided to simplify the build, run, and clean processes.

### Compilation

To compile the program, run:
```bash
make
```
or equivalently:
```bash
make COMPILE
```

### Running the Program

After compiling, you can run the program with the desired arguments. Use the following command:
```bash
make RUN ARGS="-n <matrix_size> -s <scalar_value>"
```
Replace <matrix_size> with the desired size (a non-negative integer) and <scalar_value> with the desired scalar for multiplication.

Example:
```bash
make RUN ARGS="-n 3 -s 10"
```

or, equivalently, you can run the program directly without using the Makefile after compiling as follows:

```bash
./hw0 -n 3 -s 10
```

### Cleaning Up

To clean up the build files, run:
```bash
make CLEAN
```

## Additional Documentation
For the full documentation of the project, please visit [here](https://tuna-onguner.github.io/docs/cs481/hw0/index.html).

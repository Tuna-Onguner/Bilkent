# CS481 Bioinformatics Algorithms - Homework #1


## Project Information
* Author: Deniz Tuna Onguner
* Bilkent ID: 22001788
* Semester: Spring 2025
* Section: 1
* Instructor: Can Alkan
* Date: 24.02.2025 (DD.MM.YYYY)


## Objective
The aim of this homework is to study and practice pattern searching using a suffix tree, specifically for genomic data.


## Implementation Details

### Language, Compiler, and Libraries
The implementation is written in C++17 and utilizes the following libraries: `string`, `vector`, `fstream`, `map`, `unistd.h`, and `iostream`.

### File Structure
The project is organized into the following files:
`main.cpp`: The main driver program that reads the input files, constructs the suffix tree, and searches for patterns.
`SuffixTree.h`: The header file containing the class definition for the suffix tree.
`SuffixTree.cpp`: The implementation file containing the methods for constructing the suffix tree.

### Build Instructions
A makefile is provided to compile the program. To build the program, simply run the following command:
```
make COMPILE
```

### Execution Instructions
To run the program, execute the following command:
```
MAKE RUN ARGS="-r <reference_file> -p <pattern_file> -o <output_file> [-d]"
```
or, equivalently:
```
./hw1 -r <reference_file> -p <pattern_file> -o <output_file> [-d]
```

### Clean Instructions
To clean the compiled files, run the following command:
```
make CLEAN
```
or, run the following command to remove the compiled files and the output files:
```
make CLEANALL
```


## Documentation
In case needed, a doxygen documentation can be found at the following url: https://tuna-onguner.github.io/docs/cs481/hw1/index.html.


## Command Line Arguments
The program accepts the following command line arguments:

* `-r`: Filename containing the reference sequences to be searched, in FASTA format.
* `-p`: Filename containing the patterns to search for, in FASTA format.
* `-o`: Filename to write the output results to, in text format.
* `-d`: [Optional] Flag to generate a DOT file representing the suffix tree.


## Output
The program generates the following output files:

* A `.txt` file containing the results of the pattern searches.
* If the `-d` flag is provided, an additional DOT language output file representing the suffix tree is generated.


## Algorithm
This implementation utilizes **Ukkonen's algorithm** to construct the suffix tree efficiently.


## References
The implementation is based on the following sources:

* **Original Paper:** "On-line construction of suffix trees" by Esko Ukkonen
  - [Paper Link](https://www.cs.helsinki.fi/u/ukkonen/SuffixT1withFigs.pdf)
* **Additional Resources:**
  - [GeeksForGeeks - Ukkonen's Suffix Tree Construction](https://www.geeksforgeeks.org/ukkonens-suffix-tree-construction-part-6/)
  - [StackOverflow Discussion](https://stackoverflow.com/questions/9452701/ukkonens-suffix-tree-algorithm-in-plain-english)

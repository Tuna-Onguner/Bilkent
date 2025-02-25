/*
 * Bilkent University
 * Semester of Spring 2025
 *
 * Department of Computer Engineering
 * CS481 Bioinformatics Algorithms
 *
 * Homework Assignment #1: Suffix Tree for Genomic Dataset
 *
 * Section: 1
 * Instructor: Can ALKAN
 *
 * Author: Deniz Tuna ONGUNER (Bilkent ID: 22001788)
 * Date: 23.02.2025 (DD/MM/YYYY)
 *
 * main.cpp:
 * Main file for Homework Assignment #1, pattern search in a suffix tree.
 *
 * Compiler: C++17 or later
 */

#include <map>          // Used to map headers to sequences, and patterns to results
#include <unistd.h>     // Used to parse command-line arguments
#include <iostream>     // Used for input/output operations, e.g., reading from and writing to files
#include "SuffixTree.h"

/**
 * @brief Read sequences from a FASTA file and map the headers to the sequences.
 * @param filename The name of the file to read the sequences from.
 * @return A map of headers to sequences read from the FASTA file.
 */
std::map<std::string, std::string> read_sequences_from_fasta_and_map(const std::string &filename) {
    std::ifstream fasta(filename);
    if (!fasta.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }

    std::map<std::string, std::string> sequences;
    std::string line, header, current_sequence;

    while (std::getline(fasta, line)) {
        if (line.empty()) {
            // Skip empty lines if any
            continue;
        }

        if (line[0] == '>') {
            // Header line
            if (!header.empty()) {
                sequences[header] = current_sequence;
                current_sequence.clear();
            }

            header = line.substr(1); // Skip the '>' character
        } else {
            current_sequence += line;
        }
    }

    // Add the last sequence to the map
    if (!header.empty()) {
        sequences[header] = current_sequence;
    }

    fasta.close();

    return sequences;
}

/**
 * @brief Main function of the program.
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return 0 if the program runs successfully, 1 otherwise
 * @details Expected arguments:
 *      -r: Filename to the file containing the reference sequences to be searched in FASTA format
 *      -p: Filename to the file containing the patterns to search for in FASTA format
 *      -o: Filename to write the output to in text format
 *      -d: [Optional] flag to generate a DOT file for the suffix tree
 */
int main(const int argc, char **argv) {
    std::string r_file;
    std::string p_file;
    std::string o_file;

    bool d_flag = false; // Set -d flag to false by default since it is optional

    int opt;
    while ((opt = getopt(argc, argv, "r:p:o:d")) != -1) {
        switch (opt) {
            /*
             * ASCII values of the flags:
             * -r: 114
             * -p: 112
             * -o: 111
             * -d: 100
             */
            case 114: // r
                r_file = optarg;
                break;
            case 112: // p
                p_file = optarg;
                break;
            case 111: // o
                o_file = optarg;
                break;
            case 100: // d
                d_flag = true;
                break;
            default: // Invalid flag
                std::cerr << "Usage: " << argv[0] << " -r <r_file> -p <p_file> -o <o_file> [-d]" << std::endl;
                return 1;
        }
    }

    if (r_file.empty() || p_file.empty() || o_file.empty()) {
        // If any of the required arguments are missing, print an error message and exit
        // All files are required while the -d flag is optional

        std::cerr << "Error: Missing required arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " -r <r_file> -p <p_file> -o <o_file> [-d]" << std::endl;

        return 1;
    }

    std::ofstream dot_file, out_file;

    if (d_flag) {
        // Open the DOT file for writing if the -d flag is set to true
        dot_file.open(o_file + ".dot");
        if (!dot_file.is_open()) {
            std::cerr << "Error: Could not write DOT file" << std::endl;
            return 1;
        }

        // Initialize the DOT file with the graph declaration
        dot_file << "digraph SuffixTree {" << std::endl;
    }

    const auto sequences = read_sequences_from_fasta_and_map(r_file);
    const auto patterns = read_sequences_from_fasta_and_map(p_file);

    std::map<std::string, std::string> results;

    for (const auto &[h_seq, seq]: sequences) {
        // For each sequence, build the suffix tree and search for the patterns
        SuffixTree tree(seq + "$", h_seq); // Append '$' to the end of the sequence
        tree.build_suffix_tree(); // Build the suffix tree
        // Build the suffix tree can be done in the constructor as well, but it is separated for clarity

        if (d_flag) {
            // Generate the DOT file for every suffix tree
            int id = 0;
            tree.generate_dot_file(tree.get_root(), dot_file, id);
        }

        for (const auto &[h_pat, pat]: patterns) {
            const auto pattern_indices = tree.pattern_search(pat);

            if (pattern_indices.empty()) {
                // If the pattern is not found in the sequence, continue to the next pattern
                results[h_pat] += "";
            }

            for (const auto &index: pattern_indices) {
                if (!results[h_pat].empty()) {
                    // If the result is not empty, append a comma and a space before adding the new index;
                    //      this is to separate the indices in the output file except for the last one.
                    results[h_pat] += ", ";
                }

                results[h_pat] += h_seq + ":" + std::to_string(index);
            }
        }
    }

    // Write the results to the output file
    out_file.open(o_file + ".txt");
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << o_file << std::endl;
        return 1;
    }

    for (const auto &[h_pat, result]: results) {
        // Write the results to the output file from the mapped results
        out_file << "(" << h_pat << ")" << " - " << result << std::endl;
    }

    out_file.close();

    if (d_flag) {
        // Close the DOT language syntax after writing the suffix trees
        dot_file << "}";
        // Close the DOT file after writing the suffix tree to save the changes
        dot_file.close();
    }

    return 0;
}

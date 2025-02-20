#include <map>
#include <fstream>
#include <unistd.h>
#include <iostream>

#include "SuffixTree.h"

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

int main(const int argc, char **argv) {
    /*
     * ASSUMPTIONS:
     * The reference and patterns will be provided in FASTA files.
     * The reference file will use a multi-line structure with a fixed length (60), while each pattern in the file will
     *      be stored on a single line (regardless of the string’s length). Both input files are valid.
     * There is no limit on length. The length of the reference can vary from hundreds to millions of characters.
     *      However, the length of the pattern string will be strictly less than or equal to the size of the reference.
     * You may assume that the alphabet is Σ = {A, G, C, T }.
     * The reference does not end with a terminator ($), but you will need to append it to the end yourself during
     *      the construction of the suffix tree.
     * In order to keep it simple, for each sequence, create a separate suffix tree. Store the trees in
     *      array/vector/list and perform a query for each one of them.
     * Patterns and their corresponding indices can be reported in any order.
     * The order of node and edge descriptions in the DOT file does not matter as long as the file is consistent and
     *      correctly formatted.
     */

    std::string r_file;
    std::string p_file;
    std::string o_file;
    std::string d_file;

    bool d_flag = false; // Set -d flag to false by default since it is optional

    size_t opt;
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

    const auto sequences = read_sequences_from_fasta_and_map(r_file);
    const auto patterns = read_sequences_from_fasta_and_map(p_file);

    std::map<std::string, std::string> results;

    for (const auto &[h_seq, seq]: sequences) {
        SuffixTree tree(seq + "$");
        tree.build();

        for (const auto &[h_pat, pat]: patterns) {
            const auto pattern_indices = tree.search_pattern(pat);

            if (pattern_indices.empty()) {
                results[h_pat] = "";
                continue;
            }

            for (const auto &index: pattern_indices) {
                if (results[h_pat].empty()) {
                    results[h_pat] = h_seq + ":" + std::to_string(index);
                } else {
                    results[h_pat] += ", " + h_seq + ":" + std::to_string(index);
                }
            }
        }
    }

    // Write the results to the output file
    std::ofstream output(o_file);
    if (!output.is_open()) {
        std::cerr << "Error: Could not open file " << o_file << std::endl;
        return 1;
    }

    for (const auto &[h_pat, result]: results) {
        output << h_pat << " - " << result << std::endl;
    }

    output.close();

    return 0;
}

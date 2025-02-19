#include <map>
#include <string>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <unordered_map>

namespace SuffixTree {
    struct SuffixTreeNode {
        std::unordered_map<char,
            SuffixTreeNode *> children;
        SuffixTreeNode *suffix_link;
        size_t start, *end, suffix_index;

        SuffixTreeNode(const size_t start, size_t *end) : suffix_link(nullptr), start(start),
                                                          end(end), suffix_index(-1) {
            /* Empty constructor */
        }

        ~SuffixTreeNode() {
            for (auto &[key, child]: children) {
                delete child;
            }

            children.clear();

            end = nullptr;
            suffix_link = nullptr;
        }
    };

    class SuffixTree {
    private:
        std::string reference_sequence;
        SuffixTreeNode *root, *last_new_node, *active_node;
        size_t active_edge, active_length, remaining_suffix_count, leaf_end, *root_end, *split_end;

    public:
        explicit SuffixTree(std::string reference_sequence) : reference_sequence(std::move(reference_sequence)),
                                                              last_new_node(nullptr), active_edge(-1),
                                                              active_length(0),
                                                              remaining_suffix_count(0), leaf_end(-1),
                                                              split_end(nullptr) {
            this->root_end = new size_t(-1);
            this->root = new SuffixTreeNode(-1, root_end);
            this->active_node = root;
        }

        ~SuffixTree() {
            active_node = nullptr;
            last_new_node = nullptr;

            if (split_end != nullptr) {
                delete split_end;
                split_end = nullptr;
            }
        }

        bool walk_down(SuffixTreeNode *current_node) {
            if (const size_t edge_length = 1 + *current_node->end - current_node->start; active_length >= edge_length) {
                active_edge += edge_length;
                active_length -= edge_length;
                active_node = current_node;

                return true;
            }

            return false;
        }

        void extend(const size_t position) {
            leaf_end = position;
            remaining_suffix_count += 1;
            last_new_node = nullptr;

            while (remaining_suffix_count > 0) {
                if (active_length == 0) {
                    active_edge = position;
                }

                if (active_node->children.find(reference_sequence[active_edge]) == active_node->children.end()) {
                    active_node->children[reference_sequence[active_edge]] = new SuffixTreeNode(position, &leaf_end);

                    if (last_new_node != nullptr) {
                        last_new_node->suffix_link = active_node;
                        last_new_node = nullptr;
                    }
                } else {
                    SuffixTreeNode *next = active_node->children[reference_sequence[active_edge]];

                    if (walk_down(next)) {
                        continue;
                    }

                    if (reference_sequence[next->start + active_length] == reference_sequence[position]) {
                        if (last_new_node != nullptr && active_node != root) {
                            last_new_node->suffix_link = active_node;
                            last_new_node = nullptr;
                        }

                        active_length += 1;

                        break;
                    }

                    split_end = new size_t(next->start + active_length - 1);
                    auto *split = new SuffixTreeNode(next->start, split_end);
                    active_node->children[reference_sequence[active_edge]] = split;

                    split->children[reference_sequence[position]] = new SuffixTreeNode(position, &leaf_end);
                    next->start += active_length;
                    split->children[reference_sequence[next->start]] = next;

                    if (last_new_node != nullptr) {
                        last_new_node->suffix_link = split;
                    }

                    last_new_node = split;
                    //next = nullptr;
                }

                remaining_suffix_count -= 1;

                if (active_node == root && active_length > 0) {
                    active_length -= 1;
                    active_edge = position - remaining_suffix_count + 1;
                } else if (active_node != root) {
                    active_node = active_node->suffix_link != nullptr ? active_node->suffix_link : root;
                }
            }
        }

        void build() {
            for (size_t i = 0; i < reference_sequence.size(); i += 1) {
                extend(i);
            }
        }

        [[nodiscard]] std::vector<size_t> search_pattern(const std::string &pattern) const {
            std::vector<size_t> results;
            SuffixTreeNode *current_node = root;

            size_t i = 0, j = 0;
            while (i < pattern.size()) {
                if (current_node->children.find(pattern[i]) == current_node->children.end()) {
                    return results; // Pattern not found
                }

                current_node = current_node->children[pattern[i]];

                size_t k = current_node->start;
                while (k <= *current_node->end && j < pattern.size() && reference_sequence[k] == pattern[j]) {
                    k++;
                    j++;
                    i++;
                }
                if (j == pattern.size()) {
                    results.push_back(current_node->start - j + 1);
                }
            }

            return results;
        }
    };
}

namespace FASTA {
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
        // If any of the required arguments are missing, prsize_t an error message and return 1
        // All files are required while the -d flag is optional

        std::cerr << "Error: Missing required arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " -r <r_file> -p <p_file> -o <o_file> [-d]" << std::endl;
        return 1;
    }

    // open output file or create it if it does not exist
    std::ofstream output_file(o_file);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not open file " << o_file << std::endl;
        return 1;
    }

    const std::map<std::string, std::string> reference_sequences = FASTA::read_sequences_from_fasta_and_map(r_file);
    std::cout << "Reference sequences read " << std::endl;
    const std::map<std::string, std::string> patterns = FASTA::read_sequences_from_fasta_and_map(p_file);
    std::cout << "Patterns read " << std::endl;

    for (const auto &[seq_header, sequence]: reference_sequences) {
        auto *suffix_tree = new SuffixTree::SuffixTree(sequence + '$');
        suffix_tree->build();
        std::cout << "Suffix tree built for " << seq_header << std::endl;

        for (const auto &[pattern_header, pattern]: patterns) {
            output_file << "(" << pattern_header << ")" << " - ";

            if (const std::vector<size_t> indices = suffix_tree->search_pattern(pattern); !indices.empty()) {
                for (size_t i = 0; i < indices.size(); i += 1) {
                    output_file << seq_header << ":";
                    output_file << indices[i];
                    if (i != indices.size() - 1) {
                        output_file << ", ";
                    }
                }
            }
        }

        delete suffix_tree;

        output_file << std::endl;
    }

    // Close the output file
    output_file.close();

    if (d_flag) {
        // TODO: Implement the -d flag functionality
    }

    return 0;
}

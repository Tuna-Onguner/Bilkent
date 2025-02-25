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
 * SuffixTree.cpp:
 * Implementation file for the SuffixTree class, Ukonnen's algorithm is applied.
 *
 * Compiler: C++17 or later
 *
 * IMPORTANT NOTE:
 * Ukkonen's algorithm is used to build the suffix tree in this implementation.
 * The algorithm is described in the paper "On-line construction of suffix trees" by Esko Ukkonen.
 * Refer to the paper via the following link: https://www.cs.helsinki.fi/u/ukkonen/SuffixT1withFigs.pdf.
 * Besides the original paper, the following resources were used to implement the algorithm:
 * - GeeksForGeeks: https://www.geeksforgeeks.org/ukkonens-suffix-tree-construction-part-6/
 * - StackOverflow: https://stackoverflow.com/questions/9452701/ukkonens-suffix-tree-algorithm-in-plain-english
 */

#include "SuffixTree.h"

/**
 * @brief Constructor for SuffixTreeNode.
 * @param start The start index of the suffix.
 * @param end The end index of the suffix.
 */
SuffixTreeNode::SuffixTreeNode(const int start, int *end)
    : suffix_link(nullptr), start(start), end(end), suffix_index(-1) {
    for (auto &child: children) {
        // Initialize all children to nullptr at the beginning.
        child = nullptr;
    }
}

/**
 * @brief Convert a character to an index in the alphabet.
 * @param ch The character to convert to an index.
 * @return The index of the character in the alphabet.
 * @throw std::invalid_argument if the character is not in the alphabet.
 * @details The alphabet is defined as ∑ = { A, T, C, G, $ }.
 */
int SuffixTree::char_to_index(const char ch) {
    switch (ch) {
        case 'A': return 0;
        case 'T': return 1;
        case 'C': return 2;
        case 'G': return 3;
        case '$': return 4;
        default: // Invalid character, throw an exception.
            throw std::invalid_argument("Invalid character in reference sequence `" + std::string(1, ch) + "`\n");
    }
}

/**
 * @brief Get the length of an edge.
 * @param node The node whose edge length to get.
 * @return The length of the edge.
 */
int SuffixTree::get_edge_length(const SuffixTreeNode *node) {
    return 1 + *node->end - node->start;
}

/**
 * @brief Walk down the tree from the current node.
 * @param current_node The node to walk down from.
 * @return True if the active_length is less than the edge length of the current node, false otherwise.
 */
bool SuffixTree::walk_down(SuffixTreeNode *current_node) {
    if (active_length < get_edge_length(current_node)) {
        return false;
    }

    const int edge_length = get_edge_length(current_node);

    active_edge += edge_length;
    active_length -= edge_length;
    active_node = current_node;

    return true;
}

/**
 * @brief Extend the suffix tree with the given position in the reference sequence.
 * @param position The position in the reference sequence to extend the suffix tree with.
 */
void SuffixTree::extend(const int position) {
    remaining_suffix_count += 1;
    last_new_node = nullptr;
    leaf_end = position;

    while (remaining_suffix_count > 0) {
        if (active_length == 0) {
            active_edge = position;
        }

        // If there is no outgoing edge starting with current character, create a new leaf.
        if (const int edge_index = char_to_index(reference_sequence[active_edge]);
            active_node->children[edge_index] == nullptr) {
            active_node->children[edge_index] = new SuffixTreeNode(position, &leaf_end);

            // Add suffix link if needed.
            if (last_new_node != nullptr) {
                last_new_node->suffix_link = active_node;
                last_new_node = nullptr;
            }
        } else {
            // There is an edge starting with reference_sequence[active_edge].
            SuffixTreeNode *next_node = active_node->children[edge_index];
            if (walk_down(next_node)) {
                continue;
            }

            // Check if the next character on the edge matches reference_sequence[pos].
            if (reference_sequence[next_node->start + active_length] == reference_sequence[position]) {
                // If a new internal node was created in last extension, link it.
                if (last_new_node != nullptr && active_node != root) {
                    last_new_node->suffix_link = active_node;
                    last_new_node = nullptr;
                }

                active_length += 1;
                break;
            }

            // Need to split the edge.
            split_end = new int(next_node->start + active_length - 1);
            auto *split = new SuffixTreeNode(next_node->start, split_end);
            active_node->children[edge_index] = split;

            // Create a new leaf from the split node.
            const int new_leaf_index = char_to_index(reference_sequence[position]);
            split->children[new_leaf_index] = new SuffixTreeNode(position, &leaf_end);

            // Adjust the existing child.
            next_node->start += active_length;
            const int next_index = char_to_index(reference_sequence[next_node->start]);
            split->children[next_index] = next_node;

            // Add suffix link from last_new_node if needed.
            if (last_new_node != nullptr) {
                last_new_node->suffix_link = split;
            }

            last_new_node = split;
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

/**
 * @brief Build the suffix tree from the reference sequence, and set the suffix indices for all leaves.
 */
void SuffixTree::build_suffix_tree() {
    // Build the suffix tree by iterating through the text.
    for (int i = 0; i < size; i++) {
        extend(i);
    }

    // Set suffix indices for all leaves.
    set_suffix_index_by_dfs(root);
}

/**
 * @brief Constructor for SuffixTree.
 * @param reference_sequence The reference sequence to build the suffix tree from.
 * @param header The header of the reference sequence.
 */
SuffixTree::SuffixTree(const std::string &reference_sequence, std::string header)
    : reference_sequence(reference_sequence), header(std::move(header)), root(nullptr),
      last_new_node(nullptr), active_node(nullptr), active_edge(-1), active_length(0),
      remaining_suffix_count(0), leaf_end(-1), root_end(nullptr), split_end(nullptr) {
    size = static_cast<int>(reference_sequence.size());
    root_end = new int(-1);
    root = new SuffixTreeNode(-1, root_end);
    active_node = root;
}

/**
 * @brief Get the root of the suffix tree.
 * @return The root of the suffix tree.
 */
SuffixTreeNode *SuffixTree::get_root() const {
    return root;
}

/**
 * @brief Set the suffix indices for all leaves in the suffix tree by depth-first search.
 * @param node The node to set the suffix index for.
 * @param label_height The height of the label of the node.
 */
void SuffixTree::set_suffix_index_by_dfs(SuffixTreeNode *node, const int label_height) {
    if (node == nullptr) {
        // Recursion base case.
        return;
    }

    bool is_leaf = true; // Assume the node is a leaf initially.
    for (const auto &i: node->children) {
        if (i != nullptr) {
            is_leaf = false; // The node has children, so it is not a leaf.
            set_suffix_index_by_dfs(i, label_height + get_edge_length(i)); // Recursively set suffix indices.
        }
    }

    if (is_leaf) {
        // The node is a leaf, set the suffix index to size - label_height.
        node->suffix_index = size - label_height;
    }
}

/**
 * @brief Free the memory allocated for the suffix tree.
 * @param node The node to free the memory for.
 * @details set root to the node parameter to free the entire tree.
 */
void SuffixTree::free_tree(const SuffixTreeNode *node) {
    if (node == nullptr) {
        // Recursion base case.
        return;
    }

    for (const auto i: node->children) {
        if (i != nullptr) {
            free_tree(i); // Recursively free the children.
        }
    }

    // Free allocated end pointer if it is not shared with root_end or leaf_end.
    if (node->end != root_end && node->end != &leaf_end) {
        delete node->end;
    }

    delete node;
}

/**
 * @brief Destructor for SuffixTree.
 */
SuffixTree::~SuffixTree() {
    free_tree(root);
    delete root_end;
}

/**
 * @brief Collect the leaf indices in the subtree rooted at the given node.
 * @param node The node to collect the leaf indices from.
 * @param result The vector to store the leaf indices in.
 * @details This function is used to collect the leaf indices in the subtree rooted at the given node.
 *          Which is used to find multiple occurrences of a pattern in the reference sequence.
 */
void SuffixTree::collect_leaf_indices(const SuffixTreeNode *node, std::vector<int> &result) {
    if (node == nullptr) {
        // Recursion base case.
        return;
    }

    if (node->suffix_index != -1) {
        // The node is a leaf, suffix_index != -1, add the suffix index to the result.
        result.push_back(node->suffix_index);
        return;
    }

    for (const auto child: node->children) {
        if (child != nullptr) {
            collect_leaf_indices(child, result); // Recursively collect leaf indices.
        }
    }
}

/**
 * @brief Search for a pattern in the reference sequence.
 * @param pattern The pattern to search for in the reference sequence.
 * @return A vector of indices where the pattern occurs in the reference sequence.
 */
std::vector<int> SuffixTree::pattern_search(const std::string &pattern) const {
    std::vector<int> results;
    const SuffixTreeNode *current_node = root;

    int pattern_index = 0; // i.e. the current index in the pattern.
    const int pattern_length = static_cast<int>(pattern.size());

    while (pattern_index < pattern_length) {
        const int child_index = char_to_index(pattern[pattern_index]);

        if (current_node->children[child_index] == nullptr) {
            return results; // Pattern not found, return an empty vector.
        }

        const SuffixTreeNode *child = current_node->children[child_index]; // i.e. the current child node.
        const int edge_length = get_edge_length(child); // i.e. the current length of the edge.

        int i = 0;
        // Compare the edge label with the pattern.
        while (i < edge_length && pattern_index < pattern_length) {
            if (reference_sequence[child->start + i] != pattern[pattern_index]) {
                return {}; // Mismatch, pattern not found, return an empty vector.
            }

            i += 1;
            pattern_index += 1; // Move to the next character in the pattern.
        }

        current_node = child; // Move to the child node.
    }
    // Pattern matched; now collect all leaf indices in subtree.
    collect_leaf_indices(current_node, results);
    return results;
}

/**
 * @brief Generate a DOT file for the suffix tree.
 * @param node The node to start generating the DOT file from, set to root to generate the entire tree.
 * @param dot The output stream to write the DOT file to.
 * @param id The ID of the current node.
 */
void SuffixTree::generate_dot_file(const SuffixTreeNode *node, std::ofstream &dot, int &id) const {
    if (node == nullptr) {
        // Recursion base case.
        return;
    }

    const int current_id = id++;
    const std::string current_label = "node_" + header + "_" + std::to_string(current_id);

    const bool is_leaf = node->suffix_index != -1;
    // Write the current node to the DOT file
    dot << current_label << " [label=\"" << (is_leaf ? header + ":" + std::to_string(current_id) : "") << "\"];\n";

    // Traverse the children of the current node.
    for (const auto &child: node->children) {
        if (child != nullptr) {
            const int length = get_edge_length(child);
            std::string edge_label = reference_sequence.substr(child->start, length);

            std::string child_label = "node_" + header + "_" + std::to_string(id);
            dot << current_label << " -> " << child_label << " [label=\"" << edge_label << "\"];\n";

            // Recursively generate the DOT file for the child node.
            generate_dot_file(child, dot, id);
        }
    }
}

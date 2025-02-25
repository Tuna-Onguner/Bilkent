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
 * SuffixTree.h:
 * Header file for the SuffixTree class, Ukonnen's algorithm is applied.
 *
 * Compiler: C++17 or later
 */

#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H


#include <string>
#include <vector>
#include <fstream>

/**
 * @def ALPHABET_SIZE
 * @brief The size of the alphabet used in the suffix tree.
 * @details ∑ = { A, T, C, G, $ }.
 */
#define ALPHABET_SIZE 5

struct SuffixTreeNode {
    SuffixTreeNode *children[ALPHABET_SIZE]{}, *suffix_link;
    int start, *end, suffix_index;

    explicit SuffixTreeNode(int start, int *end);
};

class SuffixTree {
public:
    explicit SuffixTree(const std::string &reference_sequence, std::string header = "");

    ~SuffixTree();

    // Public Member Functions
    [[nodiscard]] SuffixTreeNode *get_root() const;

    /* `[[nodiscard]]` is a C++11 feature indicating that the return value of the function should not be discarded. */

    void build_suffix_tree();

    [[nodiscard]] std::vector<int> pattern_search(const std::string &pattern) const;

    void generate_dot_file(const SuffixTreeNode *node, std::ofstream &, int &id) const;

private:
    // Member variables
    std::string reference_sequence, header;
    SuffixTreeNode *root, *last_new_node, *active_node;
    int active_edge, active_length, remaining_suffix_count, leaf_end, *root_end, *split_end, size;

    // Private Member Functions
    void extend(int position);

    bool walk_down(SuffixTreeNode *current_node);

    void free_tree(const SuffixTreeNode *node);

    void set_suffix_index_by_dfs(SuffixTreeNode *node, int label_height = 0);

    // Static Functions
    static int char_to_index(char ch);

    static int get_edge_length(const SuffixTreeNode *node);

    static void collect_leaf_indices(const SuffixTreeNode *node, std::vector<int> &result);
};


#endif // SUFFIX_TREE_H

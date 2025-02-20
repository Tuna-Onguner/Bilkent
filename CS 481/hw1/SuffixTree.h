#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H


#include <string>
#include <vector>
#include <unordered_map>

typedef struct SuffixTreeNode {
    std::unordered_map<char, SuffixTreeNode *> children;
    SuffixTreeNode *suffix_link;
    int start, *end, suffix_index;

    explicit SuffixTreeNode(int start, int *end);

    ~SuffixTreeNode();
} SuffixTreeNode;

class SuffixTree {
private:
    std::string reference_sequence;
    SuffixTreeNode *root, *last_new_node, *active_node;
    int active_edge, active_length, remaining_suffix_count, leaf_end;

    static int edge_length(const SuffixTreeNode *node);

    bool walk_down(SuffixTreeNode *current_node);

    void extend(int position);

public:
    explicit SuffixTree(const std::string &reference_sequence);

    ~SuffixTree();

    void build();

    [[nodiscard]] std::vector<int> search_pattern(const std::string &pattern) const;
};


#endif //SUFFIXTREE_H

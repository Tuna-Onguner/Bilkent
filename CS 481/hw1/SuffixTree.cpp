#include "SuffixTree.h"

SuffixTreeNode::SuffixTreeNode(const int start, int *end) {
    this->start = start;
    this->end = end;
    this->suffix_index = -1;
    this->suffix_link = nullptr;
}

SuffixTreeNode::~SuffixTreeNode() {
    for (auto &[key, child]: children) {
        delete child;
    }
}

int SuffixTree::edge_length(const SuffixTreeNode *node) {
    return *node->end - node->start + 1;
}

bool SuffixTree::walk_down(SuffixTreeNode *current_node) {
    if (const int edge_length = SuffixTree::edge_length(current_node); active_length >= edge_length) {
        active_edge += edge_length;
        active_length -= edge_length;
        active_node = current_node;
        return true;
    }

    return false;
}

void SuffixTree::extend(const int position) {
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

            auto *split_end = new int(next->start + active_length - 1);
            auto *split = new SuffixTreeNode(next->start, split_end);
            active_node->children[reference_sequence[active_edge]] = split;

            split->children[reference_sequence[position]] = new SuffixTreeNode(position, &leaf_end);
            next->start += active_length;
            split->children[reference_sequence[next->start]] = next;

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
            active_node = active_node->suffix_link ? active_node->suffix_link : root;
        }
    }
}

SuffixTree::SuffixTree(const std::string &reference_sequence) {
    this->reference_sequence = reference_sequence;
    root = new SuffixTreeNode(-1, new int(-1));
    active_node = root;
    active_edge = -1;
    active_length = 0;
    remaining_suffix_count = 0;
    leaf_end = -1;
    last_new_node = nullptr;
}

SuffixTree::~SuffixTree() {
    delete root;
    root = nullptr;
}

void SuffixTree::build() {
    for (int i = 0; i < reference_sequence.size(); i++) {
        extend(i);
    }
}

std::vector<int> SuffixTree::search_pattern(const std::string &pattern) const {
    std::vector<int> results;

    SuffixTreeNode *current_node = root;

    int i = 0, j = 0;
    while (i < pattern.size()) {
        if (current_node->children.find(pattern[i]) == current_node->children.end()) {
            return results; // Pattern not found
        }

        current_node = current_node->children[pattern[i]];

        int k = current_node->start;
        while (k <= *current_node->end && j < pattern.size() && reference_sequence[k] == pattern[j]) {
            k++;
            j++;
            i++;
        }

        if (j == pattern.size()) {
            results.push_back(k - j);
        }
    }

    return results;
}

/*
 * Title: Trees
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 2
 * Description: Header file for NgramTree class,
 *              which is a special binary search tree that stores ngrams and their counts.
 */

#ifndef NGRAMTREE_H
#define NGRAMTREE_H


#include <ostream> // For the << operator.

using namespace std; // Know that this is not a good practice, but it is required by the assignment.

class NgramTree {
public:
    NgramTree();

    ~NgramTree();

    void addNgram(const string &ngram);

    int getTotalNgramCount() const;

    bool isComplete() const;

    bool isFull() const;

    void generateTree(const string &fileName, const int n);

private:
    struct NgramNode {
    private:
        int count;
        string ngram;
        NgramNode *left;
        NgramNode *right;

        explicit NgramNode(const string &ngram);

        friend class NgramTree;
    };

    NgramNode *root; // Root of the tree.

    // Helper functions for the public functions.
    void destroyTree(NgramNode *node);

    int getTotalNgramCount(NgramNode *node) const;

    bool isComplete(NgramNode *node, const int index, const int totalNodeCount) const;

    bool isFull(NgramNode *node) const;

    void inOrderTraversalForOut(NgramNode *node, ostream &out) const;

    friend ostream &operator<<(ostream &out, const NgramTree &tree);
};


#endif //NGRAMTREE_H

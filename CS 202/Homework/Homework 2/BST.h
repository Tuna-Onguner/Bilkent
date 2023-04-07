/**
* Title: Binary Search Trees
* Author: Deniz Tuna Onguner
* ID: 22001788
* Section: 2
* Homework: 2
* Description: Header file for the BST class, which represents a Binary Search Tree
*/

#ifndef BST_H
#define BST_H


#include "BSTNode.h"

class BST {
public:
    BST();

    ~BST();

    // Demanded functions
    void insertItem(int key);

    void deleteItem(int key);

    int *inorderTraversal(int &length) const;

    bool hasSequence(int *seq, int length) const;

private:
    BSTNode *root;

    // Additional helper functions
    void destroyTree(BSTNode *node);

    void insertItem(BSTNode *&node, int key);

    void deleteItem(BSTNode *&node, int key);

    void inorderTraversal(BSTNode *node, int *keys, int &index, bool indicateNodes = false) const;

    int getLength(BSTNode *node) const;

    bool hasSequence(BSTNode *node, int *seq, int length) const;
};


#endif // BST_H

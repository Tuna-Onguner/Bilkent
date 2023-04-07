/**
* Title: Binary Search Trees
* Author: Deniz Tuna Onguner
* ID: 22001788
* Section: 2
* Homework: 2
* Description: Header file for the BSTNode class, which represents a single node in a Binary Search Tree
*/

#ifndef BSTNODE_H
#define BSTNODE_H


#include <iostream>

class BSTNode {
public:
    int key;
    BSTNode *left;
    BSTNode *right;

    explicit BSTNode(int key);
};


#endif // BSTNODE_H

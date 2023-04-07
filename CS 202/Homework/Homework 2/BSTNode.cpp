/**
* Title: Binary Search Trees
* Author: Deniz Tuna Onguner
* ID: 22001788
* Section: 2
* Homework: 2
* Description: Implementation file for the BSTNode class
*/

#include "BSTNode.h"

/** Constructor */
BSTNode::BSTNode(const int key) : left(nullptr), right(nullptr) {
    this->key = key;
}

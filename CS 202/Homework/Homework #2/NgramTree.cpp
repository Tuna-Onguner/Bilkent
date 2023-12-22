/*
 * Title: Trees
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 2
 * Description: Implementation file for NgramTree class,
 *              which is a special binary search tree that stores ngrams and their counts.
 */

#include <fstream> // To read the text file.
#include "NgramTree.h"

/**
 * Constructor for NgramNode.
 * @param ngram The ngram to be stored in the node.
 * @note The count is initialized to 1.
 */
NgramTree::NgramNode::NgramNode(const string &ngram) : count(1), ngram(ngram), left(NULL), right(NULL) {
    // Empty constructor.
}

/**
 * Constructor for NgramTree.
 */
NgramTree::NgramTree() : root(NULL) {
    // Empty constructor.
}

/**
 * Destructor for NgramTree.
 */
NgramTree::~NgramTree() {
    destroyTree(root); // Call of the recursive helper function.
}

/**
 * Helper function for the destructor.
 * @param node The root of the tree to be destroyed.
 * @note This function is used by the destructor.
 */
void NgramTree::destroyTree(NgramNode *node) {
    if (node != NULL) {
        destroyTree(node->left); // Destroy the left subtree.
        destroyTree(node->right); // Destroy the right subtree.
        delete node;
        node = NULL;
    }
}

/**
 * Adds a new ngram to the tree.
 * @param ngram The ngram to be added.
 */
void NgramTree::addNgram(const string &ngram) {
    if (root == NULL) { // If the tree is empty, create a new root as the very first node.
        root = new NgramNode(ngram);
    } else { // Otherwise, find the correct place to add the new node.
        NgramNode *currNode = root; // Start from the root.
        while (true) { // Loop until the new node is added or the correct place is found.
            if (ngram < currNode->ngram) { // If the new node is smaller than the current node, go left.
                if (currNode->left == NULL) { // If the left child is empty, add the new node there.
                    currNode->left = new NgramNode(ngram);
                    break;
                } else { // Otherwise, go left.
                    currNode = currNode->left;
                }
            } else if (ngram > currNode->ngram) { // If the new node is larger than the current node, go right.
                if (currNode->right == NULL) { // If the right child is empty, add the new node there.
                    currNode->right = new NgramNode(ngram);
                    break;
                } else { // Otherwise, go right.
                    currNode = currNode->right;
                }
            } else { // If the new node is equal to the current node, increment the count of the current node.
                currNode->count += 1;
                break;
            }
        }
    }
}

/**
 * Returns the total number of NgramNodes in the tree.
 * @return The total number of NgramNodes in the tree.
 */
int NgramTree::getTotalNgramCount() const {
    return getTotalNgramCount(root); // Call of the recursive helper function.
}

/**
 * Helper function for getTotalNgramCount().
 * @param node The root of the tree to be counted.
 * @return The total number of NgramNodes in the tree.
 * @note This function is used by the public getTotalNgramCount() function.
 */
int NgramTree::getTotalNgramCount(NgramNode *node) const {
    if (node == NULL) {
        return 0;
    }
    return getTotalNgramCount(node->left) + 1 + getTotalNgramCount(node->right);
}

/**
 * Checks if the tree is complete.
 * @return true if the tree is complete, false otherwise.
 */
bool NgramTree::isComplete() const {
    return isComplete(root, 0, getTotalNgramCount());
}

/**
 * Helper function for isComplete().
 * @param node The root of the tree to be checked.
 * @param index The index of the current node.
 * @param totalNodeCount The total number of nodes in the tree.
 * @return true if the tree is complete, false otherwise.
 * @note This function is used by the public isComplete() function.
 */
bool NgramTree::isComplete(NgramNode *node, const int index, const int totalNodeCount) const {
    if (node == NULL) { // If the node is empty, return true.
        return true;
    } else if (index >= totalNodeCount) { // If the index is larger than the number of nodes, return false.
        return false;
    }
    return isComplete(node->left, (2 * index + 1), totalNodeCount) && // Check the left subtree.
           isComplete(node->right, (2 * index + 2), totalNodeCount); // Check the right subtree.
}

/**
 * Checks if the tree is full.
 * @return true if the tree is full, false otherwise.
 */
bool NgramTree::isFull() const {
    return isFull(root); // Call of the recursive helper function.
}

/**
 * Helper function for isFull().
 * @param node The root of the tree to be checked.
 * @return true if the tree is full, false otherwise.
 * @note This function is used by the public isFull() function.
 */
bool NgramTree::isFull(NgramNode *node) const {
    if (node == NULL) { // If the node is empty,
        return true;
    }
    if (node->left == NULL && node->right == NULL) { // If the node is a leaf, return true.
        return true;
    }
    if (node->left && node->right) { // If the node has two children, check the subtrees.
        return isFull(node->left) && isFull(node->right);
    }
    return false; // Otherwise, return false.
}

/**
 * Generates the tree from a text file.
 * @param fileName The name of the text file.
 * @param n The n in n-grams.
 */
void NgramTree::generateTree(const string &fileName, const int n) {
    ifstream textFile(fileName.c_str()); // Open the file.

    if (!textFile.is_open()) { // If the file cannot be opened, throw an exception.
        throw runtime_error("Failed to load file: " + fileName);
    }

    string word;
    while (textFile >> word) { // Read the file word by word.
        if (((int) word.length()) < n) { // If the word is shorter than n, skip it.
            continue;
        }
        for (int i = 0; i < ((int) word.length()) - n + 1; i += 1) { // Add all n-grams of the word to the tree.
            addNgram(word.substr(i, n));
        }
    }

    textFile.close(); // Close the file.
}

/**
 * Prints the tree in order.
 * @param node The root of the tree to be printed.
 * @param out The output stream to be used.
 * @note This function is used by the overloaded << operator.
 */
void NgramTree::inOrderTraversalForOut(NgramNode *node, ostream &out) const {
    if (node != NULL) { // If the node is not empty,...
        inOrderTraversalForOut(node->left, out); // ...print the left subtree,...
        out << "\"" << node->ngram << "\"" << " appears " << node->count << " time(s)\n"; // ...the node itself,...
        inOrderTraversalForOut(node->right, out); // ...and the right subtree.
    }
}

/**
 * Overloaded << operator for NgramTree.
 * @param out The output stream to be used.
 * @param tree The tree to be printed.
 * @return The output stream.
 */
ostream &operator<<(ostream &out, const NgramTree &tree) {
    tree.inOrderTraversalForOut(tree.root, out); // Call of the recursive helper function.
    return out; // Return the output stream.
}

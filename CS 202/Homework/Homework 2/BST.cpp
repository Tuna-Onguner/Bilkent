/**
* Title: Binary Search Trees
* Author: Deniz Tuna Onguner
* ID: 22001788
* Section: 2
* Homework: 2
* Description: Implementation file for the BST class
*/

#include "BST.h"

/** Constructor */
BST::BST() {
    root = nullptr;
}

/** Destructor */
BST::~BST() {
    destroyTree(root);
}

/**
 * Recursively destroy the subtree rooted at node.
 * @param node The root node of the subtree to be destroyed.
 */
void BST::destroyTree(BSTNode *node) {
    if (node != nullptr) {
        destroyTree(node->left); // destroy left-tree
        destroyTree(node->right); // destroy right-tree
        delete node;
    }
}

/**
 * Insert a new node with the given key into the tree.
 * @param key The key of the new node to be inserted.
 */
void BST::insertItem(const int key) {
    insertItem(root, key); // call of helper function
}

/**
 * Recursively insert a new node with the given key into the subtree rooted at node.
 * @param node A reference to the root node of the subtree to insert the new node into.
 * @param key The key of the new node to be inserted.
 */
void BST::insertItem(BSTNode *&node, const int key) {
    if (node == nullptr)
        node = new BSTNode(key); // right spot, insert new node
    else if (key < node->key)
        insertItem(node->left, key); // traverse to left-tree
    else if (key > node->key)
        insertItem(node->right, key); // traverse to right-tree
}

/**
 * Delete the node with the given key from the tree.
 * @param key The key of the node to be deleted.
 */
void BST::deleteItem(const int key) {
    deleteItem(root, key); // call of helper function
}

/**
 * Recursively delete the node with the given key from the subtree rooted at node.
 * @param node A reference to the root node of the subtree to delete the node from.
 * @param key The key of the node to be deleted.
 */
void BST::deleteItem(BSTNode *&node, const int key) {
    if (node == nullptr)
        return;
    else if (key < node->key)
        deleteItem(node->left, key); // traverse to left-tree
    else if (key > node->key)
        deleteItem(node->right, key); // traverse to right-tree
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        } else if (node->left == nullptr) {
            BSTNode *temp = node;
            node = node->right;
            delete temp;
        } else if (node->right == nullptr) {
            BSTNode *temp = node;
            node = node->left;
            delete temp;
        } else {
            BSTNode *temp = node->right;
            while (temp->left != nullptr)
                temp = temp->left;
            node->key = temp->key;
            deleteItem(node->right, temp->key);
        }
    }
}

/**
 * Return an ordered array of all keys in the tree by performing an inorder traversal.
 * The length parameter is set to the length of the array.
 * @param length A reference to an integer where the length of the returned array will be stored.
 * @return An ordered array of keys in the tree.
 */
int *BST::inorderTraversal(int &length) const {
    length = getLength(root);
    if (root == nullptr)
        return nullptr;
    int *keys = new int[length], index = 0; // allocate new array to fill and return
    inorderTraversal(root, keys, index); // call of helper function
    return keys;
}

/**
 * Recursively perform an inorder traversal of the subtree
 * rooted at node and fill the given array with the keys in order.
 * @param node The root node of the subtree to be traversed.
 * @param arr A pointer to the array to be filled with the keys.
 * @param index A reference to an integer representing the index of the next empty slot in the array.
 * @param indicateNodes A condition to print nodes visited onto the console if true.
 */
void BST::inorderTraversal(BSTNode *node, int *keys, int &index, const bool indicateNodes) const {
    if (node == nullptr)
        return;
    if (indicateNodes) // print current node's value...
        printf("%d ", node->key); // ...if it is called from hasSequence function
    inorderTraversal(node->left, keys, index, indicateNodes); // traverse to left-tree
    keys[index] = node->key; // put value into array
    index++;
    inorderTraversal(node->right, keys, index, indicateNodes); // traverse to right-tree
}

/**
 * Recursively calculate the total number of nodes in the subtree rooted at node.
 * @param node The root node of the subtree to calculate the number of nodes for.
 * @return The total number of nodes in the subtree rooted at node.
 */
int BST::getLength(BSTNode *node) const {
    if (node == nullptr)
        return 0;
    return getLength(node->left) + getLength(node->right) + 1;
}

/**
 * Checks whether the binary search tree contains a given sequence of keys.
 * @param seq The sequence of keys to check for.
 * @param length The length of the sequence.
 * @return True if the tree contains the sequence, false otherwise.
 */
bool BST::hasSequence(int *seq, const int length) const {
    printf("Visited Nodes: ");
    return hasSequence(root, seq, length); // call of helper function
}

/**
 * Checks whether a given binary search tree node contains a given sequence of keys.
 * @param node The node to check for the sequence.
 * @param seq The sequence of keys to check for.
 * @param length The length of the sequence.
 * @return True if the node contains the sequence, false otherwise.
 */
bool BST::hasSequence(BSTNode *node, int *seq, const int length) const {
    if (node == nullptr) {
        return false;
    } else if (node->key > seq[length - 1]) {
        printf("%d ", node->key);
        return hasSequence(node->left, seq, length); // traverse to left-tree
    } else if (node->key < seq[0]) {
        printf("%d ", node->key);
        return hasSequence(node->right, seq, length); // traverse to right-tree
    } else { // node to start to traverse
        int index = 0, keys[getLength(node)];
        inorderTraversal(node, keys, index, true); // get nodes' values
        for (int i = 0; i < length; i++) {
            if (seq[i] != keys[i]) { // compare sequence and tree values...
                return false; // ...return false if any is not matched
            }
        }
        return true; // return true, all values are matched
    }
}

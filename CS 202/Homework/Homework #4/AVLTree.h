/*
 * Title: Balanced Search Trees
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 4
 * Description: Header file of the AVLTree class
 */

#ifndef SUMMER2023_CS202_HW4_AVL_TREE_H
#define SUMMER2023_CS202_HW4_AVL_TREE_H


#include <string>
#include <fstream>
#include <iostream>

class AVLTree {
private:
    // Node object for the AVLTree
    struct AVLNode {
    private:
        const std::string WORD;
        int frequency;
        AVLNode *left;
        AVLNode *right;
        int height;

        explicit AVLNode(const std::string &WORD) : WORD(WORD) {
            this->frequency = 1;
            this->left = nullptr;
            this->right = nullptr;
            this->height = 1;
        }

        ~AVLNode() = default;

        friend class AVLTree;
    };

    // Data members
    AVLNode *root;
    std::ofstream FileStats;
    std::ofstream FileFreqs;

    // Helper functions
    void destroyTree(AVLNode *avlNode);

    static int max(int a, int b);

    static int getNodeHeight(AVLNode *avlNode);

    static int getBalanceFactor(AVLNode *avlNode);

    static AVLNode *leftRotate(AVLNode *avlNode);

    static AVLNode *rightRotate(AVLNode *avlNode);

    void insert(AVLNode *&avlNode, const std::string &WORD);

    // Inorder traversal function as a template function
    template<typename Func>
    void inorderTraversal(AVLNode *avlNode, Func visit);

public:
    // Constructor and destructor
    AVLTree(const std::string &FILE_FREQS, const std::string &FILE_STATS);

    ~AVLTree();

    // Member functions
    void addWord(const std::string &WORD);

    void generateTree(const std::string &INPUT_FILE) noexcept(false);

    void printHeight();

    void printTotalWordCount();

    void printWordFrequencies();

    void printMostFrequent();

    void printLeastFrequent();

    void printStandartDeviation();
};


#endif //SUMMER2023_CS202_HW4_AVL_TREE_H

/*
 * Title: Balanced Search Trees
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 4
 * Description: Implementation of the AVLTree class
 */

#include <cmath> // To use std::pow() and std::sqrt() for standart deviation calculation
#include <iomanip> // To use std::setprecision() and std::fixed for standart deviation outputting

#include "AVLTree.h"

/**
 * @brief Constructor of the AVLTree class
 */
AVLTree::AVLTree(const std::string &FILE_FREQS, const std::string &FILE_STATS) : root(nullptr) {

    // Open the files
    FileFreqs.open(FILE_FREQS.c_str());
    FileStats.open(FILE_STATS.c_str());

    // Check if the files are opened successfully
    if (!FileFreqs.is_open() || !FileStats.is_open()) {
        std::cerr << "Error: Cannot open the output files!" << std::endl;
        exit(1); // Exit the program if the files cannot be opened
    }
}

/**
 * @brief Destructor of the AVLTree class
 */
AVLTree::~AVLTree() {

    // Close the files
    FileFreqs.close();
    FileStats.close();

    // Destroy the tree
    destroyTree(root);
}

/**
 * @brief Destroys the tree recursively
 * @param avlNode initially, the root of the tree to be destroyed, and then the current node
 */
void AVLTree::destroyTree(AVLNode *avlNode) {
    if (avlNode != nullptr) {
        destroyTree(avlNode->left);
        destroyTree(avlNode->right);
        delete avlNode;
        //avlNode = nullptr; Commented out because Clang-Tidy says it is unnecessary
    }
}

/**
 * @brief Returns the maximum of two integers
 * @param A the first integer
 * @param B the second integer
 * @return The maximum of the two integers
 */
int AVLTree::max(const int A, const int B) {
    return ((A > B) ? A : B);
}

/**
 * @brief Returns the height of the given node
 * @param avlNode the node whose height is to be returned
 * @return The height of the given node
 * @note If the node is nullptr, returns 0
 */
int AVLTree::getNodeHeight(AVLNode *avlNode) {
    if (avlNode == nullptr) {
        return 0;
    }
    return avlNode->height;
}

/**
 * @brief Returns the balance factor of the given node
 * @param avlNode the node whose balance factor is to be returned
 * @return The balance factor of the given node
 * @note If the node is nullptr, returns 0
 */
int AVLTree::getBalanceFactor(AVLNode *avlNode) {
    if (avlNode == nullptr) {
        return 0;
    }
    return (getNodeHeight(avlNode->left) - getNodeHeight(avlNode->right));
}

/**
 * @brief Performs a left rotation on the given node
 * @param avlNode the node to be rotated
 * @return The new root of the subtree
 */
AVLTree::AVLNode *AVLTree::leftRotate(AVLNode *avlNode) {

    AVLNode *rightChild = avlNode->right; // Get the right child of the node
    AVLNode *rightLeftChild = rightChild->left; // Get the left child of the right child of the node

    rightChild->left = avlNode; // Make the node the left child of the right child of the node
    avlNode->right = rightLeftChild; // Make the left child of the right child of the node the right child of the node

    avlNode->height = 1 + max(getNodeHeight(avlNode->left), getNodeHeight(avlNode->right));
    rightChild->height = 1 + max(getNodeHeight(rightChild->left), getNodeHeight(rightChild->right));

    return rightChild;
}

/**
 * @brief Performs a right rotation on the given node
 * @param avlNode the node to be rotated
 * @return The new root of the subtree
 */
AVLTree::AVLNode *AVLTree::rightRotate(AVLNode *avlNode) {

    AVLNode *leftChild = avlNode->left; // Get the left child of the node
    AVLNode *leftRightChild = leftChild->right; // Get the right child of the left child of the node

    leftChild->right = avlNode; // Make the node the right child of the left child of the node
    avlNode->left = leftRightChild; // Make the right child of the left child of the node the left child of the node

    avlNode->height = 1 + max(getNodeHeight(avlNode->left), getNodeHeight(avlNode->right));
    leftChild->height = 1 + max(getNodeHeight(leftChild->left), getNodeHeight(leftChild->right));

    return leftChild;
}

/**
 * @brief Inserts the given word to the tree
 * @param WORD the word to be inserted
 * @note Time complexity := O(log(n))
 */
void AVLTree::addWord(const std::string &WORD) {
    insert(root, WORD);
}

/**
 * @brief Inserts the given word to the tree recursively
 * @param avlNode the root of the tree
 * @param WORD the word to be inserted
 * @note Time complexity := O(log(n))
 */
void AVLTree::insert(AVLNode *&avlNode, const std::string &WORD) {

    // Case 1: If the tree is empty, create a new node and make it the root
    if (avlNode == nullptr) {
        avlNode = new AVLNode(WORD);
        return;
    }

    // Case 2: The tree is not empty, so insert the word to the left or right subtree
    // Case 2.1: Insert to the left subtree
    if (WORD < avlNode->WORD) {
        insert(avlNode->left, WORD);
    }

    // Case 2.2: Insert to the right subtree
    else if (WORD > avlNode->WORD) {
        insert(avlNode->right, WORD);
    }

    // Case 2.3: The word is already in the tree, so increment its frequency
    else {
        avlNode->frequency += 1;
        return; // Return because the word is already in the tree
    }

    // Update the height of the node
    avlNode->height = 1 + max(getNodeHeight(avlNode->left), getNodeHeight(avlNode->right));

    // Get the balance factor of the node
    const int BALANCE_FACTOR = getBalanceFactor(avlNode);

    // Case 3: The node is unbalanced, so perform the necessary rotations
    // Case 3.1: Single Left Rotation
    if (BALANCE_FACTOR > 1 && WORD < avlNode->left->WORD) {
        avlNode = rightRotate(avlNode);
    }

    // Case 3.2: Single Right Rotation
    else if (BALANCE_FACTOR < -1 && WORD > avlNode->right->WORD) {
        avlNode = leftRotate(avlNode);
    }

    // Case 3.3: Left Right Rotation
    else if (BALANCE_FACTOR > 1 && WORD > avlNode->left->WORD) {
        avlNode->left = leftRotate(avlNode->left);
        avlNode = rightRotate(avlNode);
    }

    // Case 3.4: Right Left Rotation
    else if (BALANCE_FACTOR < -1 && WORD < avlNode->right->WORD) {
        avlNode->right = rightRotate(avlNode->right);
        avlNode = leftRotate(avlNode);
    }
}

/**
 * @brief Generates the tree from the given file
 * @param INPUT_FILE the name of the file to be read
 * @throws runtime_error if the file cannot be opened
 * @note Time complexity := O(n)
 */
void AVLTree::generateTree(const std::string &INPUT_FILE) noexcept(false) {

    std::ifstream file(INPUT_FILE.c_str()); // Open the file
    if (!file.is_open()) { // If the file cannot be opened, throw an exception
        throw std::runtime_error("File '" + INPUT_FILE + "' cannot be opened!");
    }

    std::string word;
    while (file >> word) { // Read the file word by word
        std::string wordToAdd; // Create a string to add to the tree
        for (const char CHARACTER: word) { // Iterate over the word
            if (std::isalnum(CHARACTER)) { // If the character is alphanumeric, add it to the word
                wordToAdd.push_back(CHARACTER);
            } else { // If the character is not alphanumeric, i.e. punctuation, add the word to the tree
                if (!wordToAdd.empty()) { // If the word is not empty, add it to the tree
                    addWord(wordToAdd);
                    wordToAdd.clear(); // Clear the word for the next iterations
                }
            }
        }
        if (!wordToAdd.empty()) { // If the word is not empty, add it to the tree
            addWord(wordToAdd);
        }
    }

    file.close(); // Close the file
}

/**
 * @brief Prints the height of the tree to the statistics file
 * @note Time complexity := O(1)
 */
void AVLTree::printHeight() {
    if (root == nullptr) {
        FileStats << "Tree Height: 0" << std::endl;
        return;
    }
    FileStats << "Tree Height: " << root->height << std::endl;
}

/**
 * @brief Prints the total word count to the statistics file
 * @note Time complexity := O(n)
 */
void AVLTree::printTotalWordCount() {

    int totalWordCount = 0;

    // Traverse the tree and increment the total word count
    inorderTraversal(root, [&totalWordCount](AVLNode *) {
        totalWordCount += 1;
    });

    // Print the total word count to the statistics file
    FileStats << "Total Word Count: " << totalWordCount << std::endl;
}

/**
 * @brief Prints the word frequencies to the frequencies file
 * @note Time complexity := O(n)
 */
void AVLTree::printWordFrequencies() {

    // Traverse the tree and print the word frequencies to the frequencies file
    inorderTraversal(root, [&](AVLNode *avlNode) {
        FileFreqs << avlNode->WORD << ": " << avlNode->frequency << std::endl;
    });
}

/**
 * @brief Prints the most frequent word to the statistics file
 * @note Time complexity := [2 * O(n)] = O(n)
 */
void AVLTree::printMostFrequent() {

    int mostFrequency = root->frequency; // Initialize the most frequent word as the root

    // Traverse the tree to find the most frequent word
    inorderTraversal(root, [&mostFrequency](AVLNode *avlNode) {
        if (avlNode->frequency > mostFrequency) {
            mostFrequency = avlNode->frequency;
        }
    });

    FileStats << "Most Frequent: " << mostFrequency << ":";

    inorderTraversal(root, [&, mostFrequency](AVLNode *avlNode) {
        if (avlNode->frequency == mostFrequency) {
            FileStats << " " << avlNode->WORD;
        }
    });

    FileStats << std::endl;
}

/**
 * @brief Prints the least frequent word to the statistics file
 * @note Time complexity := [2 * O(n)] = O(n)
 */
void AVLTree::printLeastFrequent() {

    int leastFrequency = root->frequency; // Initialize the least frequent word as the root

    // Traverse the tree to find the least frequent word
    inorderTraversal(root, [&leastFrequency](AVLNode *avlNode) {
        if (avlNode->frequency < leastFrequency) {
            leastFrequency = avlNode->frequency;
        }
    });

    FileStats << "Least Frequent: " << leastFrequency << ":";

    inorderTraversal(root, [&, leastFrequency](AVLNode *avlNode) {
        if (avlNode->frequency == leastFrequency) {
            FileStats << " " << avlNode->WORD;
        }
    });

    FileStats << std::endl;
}

/**
 * @brief Calculates the standart deviation of the tree and prints it to the statistics file
 * @note Time complexity := [2 * O(n)] = O(n)
 */
void AVLTree::printStandartDeviation() {

    // Traverse the tree to calculate the mean := sum of all frequencies / total word count
    int totalWordCount = 0, sumOfWordFrequencies = 0;
    inorderTraversal(root, [&totalWordCount, &sumOfWordFrequencies](AVLNode *avlNode) {
        totalWordCount = totalWordCount + 1;
        sumOfWordFrequencies += avlNode->frequency;
    });
    const double MEAN = (double) sumOfWordFrequencies / totalWordCount;

    // Traverse the tree once more to calculate the sum of squares := sum of (frequency - mean) ** 2
    double sumOfSquares = 0.0;
    inorderTraversal(root, [MEAN, &sumOfSquares](AVLNode *avlNode) {
        sumOfSquares += pow((avlNode->frequency - MEAN), 2);
    });

    // Calculate the standart deviation := sqrt(sum of squares / total word count)
    const double STANDART_DEVIATION = sqrt(sumOfSquares / totalWordCount);
    FileStats << "Standart Deviation: " << std::fixed << std::setprecision(2)
              << STANDART_DEVIATION << std::endl; // Print the standart deviation to the statistics file
}

/**
 * @brief Traverses the tree in inorder fashion and calls the given function for each node
 * @tparam Func the type of the function to be called for each node
 * @param avlNode initially the root of the tree, and then the current node
 * @param visit the function to be called for each node
 * @note Time complexity := O(n)
 */
template<typename Func>
void AVLTree::inorderTraversal(AVLNode *avlNode, Func visit) {
    if (avlNode == nullptr) { // If the current node is null, return
        return;
    } else {
        inorderTraversal(avlNode->left, visit); // Traverse the left subtree
        visit(avlNode); // Call the given function for the current node
        inorderTraversal(avlNode->right, visit); // Traverse the right subtree
    }
}

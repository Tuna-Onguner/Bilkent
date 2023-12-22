/*
 * Title: Balanced Search Trees
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 4
 * Description: Main file of the program
 */

#include "AVLTree.h"

int main(int argc, char **argv) {

    // Check if the user has provided a file name
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_filename>" << std::endl;
        return 1; // Exit with error code if no file name is provided
    }

    const std::string FILE_FREQS = "wordfreqs";
    const std::string FILE_STATS = "statistics";

    const std::string INPUT_FILE = argv[1]; // Get the file name from the user

    // Create an AVLTree object
    auto *avlTree = new AVLTree(FILE_FREQS, FILE_STATS);
    avlTree->generateTree(INPUT_FILE); // Generate the tree from the file provided by the user

    // Print the results asked
    avlTree->printWordFrequencies();
    avlTree->printTotalWordCount();
    avlTree->printHeight();
    avlTree->printMostFrequent();
    avlTree->printLeastFrequent();
    avlTree->printStandartDeviation();

    delete avlTree; // Delete the AVLTree object

    return 0; // Exit with success code
}

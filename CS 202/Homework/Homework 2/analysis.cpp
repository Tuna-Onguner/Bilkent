/**
* Title: Binary Search Trees
* Author: Deniz Tuna Onguner
* ID: 22001788
* Section: 2
* Homework: 2
* Description: Implementation file for the analysis
*/

#include "analysis.h"

void timeAnalysis() {

    BST bst;

    const int NUMBER_OF_COMPUTATIONS = 15000;
    int arrayRandomKeys[NUMBER_OF_COMPUTATIONS];

    for (int &arrayRandomKey: arrayRandomKeys)
        arrayRandomKey = rand();

    // Part 1
    std::cout << "Part e - Time analysis of Binary Search Tree - part 1\n"
                 "-----------------------------------------------------\n"
                 "Tree Size             Time elapsed\n"
                 "-----------------------------------------------------\n";

    clock_t startTime = clock();

    for (int i = 0; i < NUMBER_OF_COMPUTATIONS; i++) {
        bst.insertItem(arrayRandomKeys[i]);
        if ((i + 1) % 1500 == 0) {
            clock_t endTime = clock();
            printf("%5d %24f ms\n", (i + 1), (1000 * (double) (endTime - startTime) / CLOCKS_PER_SEC));
        }
    }

    // Online source applied for shuffling: https://www.geeksforgeeks.org/shuffle-vs-random_shuffle-c/
    unsigned int seed = 0;
    shuffle(arrayRandomKeys, arrayRandomKeys + NUMBER_OF_COMPUTATIONS,
            std::default_random_engine(seed));

    // Part 2
    std::cout << "\nPart e - Time analysis of Binary Search Tree - part 2"
                 "\n-----------------------------------------------------"
                 "\nTree Size             Time elapsed\n"
                 "-----------------------------------------------------\n";

    startTime = clock();

    for (int i = 0; i < NUMBER_OF_COMPUTATIONS; i++) {
        bst.deleteItem(arrayRandomKeys[i]);
        if ((i + 1) % 1500 == 0) {
            clock_t endTime = clock();
            printf("%5d %24f ms\n", (NUMBER_OF_COMPUTATIONS - i - 1),
                   (1000 * (double) (endTime - startTime) / CLOCKS_PER_SEC));
        }
    }
}

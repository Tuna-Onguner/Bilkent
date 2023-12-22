/*
 * Title: Trees
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 2
 * Description: main.cpp file for testing the NgramTree class.
 */

#include <cstdlib>
#include <iostream>
#include "NgramTree.h"

int main(int argc, char **argv) {

    if (argc != 3) {
        cout << "Usage: ./hw2 [filename] [n]" << endl;
        return 1;
    }

    const int n = atoi(argv[2]);

    NgramTree tree;
    tree.generateTree(string(argv[1]), n);

    cout << "Total " << n << "-gram count: " << tree.getTotalNgramCount() << endl;
    cout << tree << endl;
    cout << n << "-gram tree is complete: " << (tree.isComplete() ? "Yes" : "No") << endl;
    cout << n << "-gram tree is full: " << (tree.isFull() ? "Yes" : "No") << endl;

    tree.addNgram("samp");
    tree.addNgram("samp");
    tree.addNgram("zinc");
    tree.addNgram("aatt");

    cout << "\nTotal " << n << "-gram count: " << tree.getTotalNgramCount() << endl;
    cout << tree << endl;
    cout << n << "-gram tree is complete: " << (tree.isComplete() ? "Yes" : "No") << endl;
    cout << n << "-gram tree is full: " << (tree.isFull() ? "Yes" : "No") << endl;

    return 0;
}

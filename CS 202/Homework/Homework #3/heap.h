/*
 * Title: Heaps and AVL Tree
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 3
 * Description: Header of Heap class; declarations of Heap class functions and heapRebuild function.
 */

#ifndef SUMMER2023_CS202_HW3_HEAP_H
#define SUMMER2023_CS202_HW3_HEAP_H


#include <string> //To use string objects
#include <exception> //To implement HeapException class

void heapRebuild(int *items, int root, int last, int &comparisonCount);

class Heap {
private:
    static const int MAX_HEAP = 10000; //Assume a maximum capacity of 10,000 items

    class HeapException : std::exception {
    private:
        const std::string MESSAGE;

    public:
        const char *what() const noexcept override {
            return MESSAGE.c_str();
        }

        explicit HeapException(const std::string &MESSAGE = "") : std::exception(), MESSAGE(MESSAGE) {
            /* Empty constructor */
        }

        ~HeapException() noexcept override = default;
    };

    int size; //Current number of items in the heap
    int items[MAX_HEAP];
    int comparisonCount;

public:
    Heap();

    bool isEmpty() const;

    void insert(int newItem) noexcept(false);

    int maximum() const noexcept(false);

    int popMaximum() noexcept(false);

    int getComparisonCount() const;
};


#endif //SUMMER2023_CS202_HW3_HEAP_H

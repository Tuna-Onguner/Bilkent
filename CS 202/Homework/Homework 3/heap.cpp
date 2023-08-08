/*
 * Title: Heaps and AVL Tree
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 3
 * Description: Implementations of Heap class functions and heapRebuild function.
 */

#include "heap.h"

/**
 * Constructor
 */
Heap::Heap() : size(0), items(), comparisonCount(0) {
    /* Empty constructor */
}

/**
 * @brief Checks if the heap is empty.
 * @return True if the heap is empty, false otherwise.
 */
bool Heap::isEmpty() const {
    return size <= 0;
}

/**
 * @brief Inserts a new item into the heap.
 * @param newItem the item to be inserted in.
 * @throws HeapException 'Heap full' if the heap is full.
 * @note Time complexity: O(log(n))
 */
void Heap::insert(const int newItem) noexcept(false) {
    if (size >= MAX_HEAP) {
        throw HeapException("HeapException: Heap full");
    }
    items[size] = newItem;
    int place = size;
    int parent = (place - 1) / 2;
    while (place > 0 && items[place] > items[parent]) {
        comparisonCount++; //items[place] > items[parent]
        const int temp = items[parent];
        items[parent] = items[place];
        items[place] = temp;
        place = parent;
        parent = (place - 1) / 2;
    }
    size++;
}

/**
 * @brief Returns the maximum item in the heap.
 * @return The maximum item in the heap.
 * @throws HeapException 'Heap empty' if the heap is empty.
 * @note Time complexity: O(1)
 */
int Heap::maximum() const noexcept(false) {
    if (isEmpty()) {
        throw HeapException("HeapException: Heap empty");
    }
    return items[0];
}

/**
 * @brief Removes the maximum item in the heap.
 * @return The maximum item in the heap.
 * @throws HeapException 'Heap empty' if the heap is empty.
 * @note Time complexity: O(log(n))
 */
int Heap::popMaximum() noexcept(false) {
    if (isEmpty()) {
        throw HeapException("HeapException: Heap empty");
    }
    const int root = items[0];
    items[0] = items[--size];
    heapRebuild(items, 0, size, comparisonCount);
    return root;
}

/**
 * @brief Returns the number of comparisons made.
 * @return The number of comparisons made.
 */
int Heap::getComparisonCount() const {
    return comparisonCount;
}

/**
 * @brief Rebuilds the heap.
 * @param items the array containing the heap.
 * @param root the root of the heap.
 * @param last the last item in the heap.
 * @param comparisonCount the number of comparisons made.
 * @note Time complexity: O(log(n))
 */
void heapRebuild(int *items, const int root, const int last, int &comparisonCount) {
    int child = 2 * root + 1;
    if (child < last) {
        const int rightChild = child + 1;
        if (rightChild < last && items[rightChild] > items[child]) {
            comparisonCount++; //1st comparison: items[rightChild] > items[child]
            child = rightChild;
        }
        comparisonCount++; //2nd comparison: items[root] < items[child]
        if (items[root] < items[child]) {
            const int temp = items[root];
            items[root] = items[child];
            items[child] = temp;
            heapRebuild(items, child, last, comparisonCount);
        }
    }
}

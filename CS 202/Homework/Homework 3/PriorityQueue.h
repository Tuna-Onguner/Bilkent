/**
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 2
 * Homework: 3
 * Description: Implementation of the binary heap based priority queue as a template class.
 *
 * References:
 * https://www.geeksforgeeks.org/priority-queue-using-binary-heap/
 * https://www.geeksforgeeks.org/templates-cpp/
 * https://www.geeksforgeeks.org/exception-handling-c/
 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H


/**
 * @brief A priority queue implementation using a binary heap.
 *
 * @tparam PriorityQueueType The type of elements stored in the priority queue.
 * @tparam maxHeapSize The maximum element count that the heap can store.
 */
template<typename PriorityQueueType, unsigned const int maxHeapSize>
class PriorityQueue {
private:
    unsigned int currentHeapSize;
    PriorityQueueType heap[maxHeapSize];

    /**
     * @brief Moves an item up the heap to maintain the heap property.
     *
     * @param index The index of the item to move up.
     */
    void heapUp(unsigned const int index) {

        if (index == 0)
            return;

        unsigned const int parentOfIndex = (index - 1) / 2;

        if (heap[index] < heap[parentOfIndex]) {
            std::swap(heap[index], heap[parentOfIndex]);
            heapUp(parentOfIndex);
        }
    }

    /**
     * @brief Moves an item down the heap to maintain the heap property.
     *
     * @param index The index of the item to move down.
     */
    void heapDown(unsigned const int index) {

        unsigned int theSmallest = index;

        unsigned const int leftSub = (2 * index + 1),
                rightSub = (2 * index + 2);

        if (leftSub < currentHeapSize && heap[leftSub] < heap[theSmallest])
            theSmallest = leftSub;

        if (rightSub < currentHeapSize && heap[rightSub] < heap[theSmallest])
            theSmallest = rightSub;

        if (theSmallest != index) {
            std::swap(heap[index], heap[theSmallest]);
            heapDown(theSmallest);
        }
    }

public:
    /**
     * @brief Constructors a new PriorityQueue object.
     */
    PriorityQueue() : currentHeapSize(0), heap() {
        // pass
    }

    /**
     * @brief Adds a new item to the heap.
     *
     * @param newItem The item to add to the priority queue.
     * @throws runtime_error If the heap is full.
     */
    void push(const PriorityQueueType newItem) {

        if (currentHeapSize == maxHeapSize)
            throw std::runtime_error("Heap is full.");

        heap[currentHeapSize++] = newItem;
        heapUp(currentHeapSize - 1);
    }

    /**
     * @brief Removes the item with the highest priority from the heap.
     *
     * @throws runtime_error If the heap is empty.
     */
    void pop() {

        if (currentHeapSize == 0)
            throw std::runtime_error("Heap is empty.");

        heap[0] = heap[--currentHeapSize];
        heapDown(0);
    }

    /**
     * @brief Returns a reference to the item with the highest priority.
     *
     * @return A reference to the item with the highest priority.
     * @throws runtime_error If the heap is empty.
     */
    PriorityQueueType &top() {

        if (currentHeapSize == 0)
            throw std::runtime_error("Heap is empty.");

        return heap[0];
    }

    /**
     * @brief Checks if the heap is empty.
     *
     * @return True if the heap is empty, false otherwise.
     */
    bool empty() const {
        return (currentHeapSize == 0);
    }
};


#endif // PRIORITY_QUEUE_H

/**
 * Title: Algorithm analysis & sorting
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 2
 * Homework: 1
 * Description: Sorting class, the header of the sorting.cpp
 */

#ifndef SORTING_H
#define SORTING_H


class Sorting {
public:
    void insertionSort(int *arr, const int size, int &compCount, int &moveCount);

    void bubbleSort(int *arr, const int size, int &compCount, int &moveCount);

    void mergeSort(int *arr, const int size, int &compCount, int &moveCount);

    void quickSort(int *arr, const int size, int &compCount, int &moveCount);

    void hybridSort(int *arr, const int size, int &compCount, int &moveCount);

private:
    void swap(int *, int *);

    void bubbleSort(int *, int, int, int &, int &);

    void merge(int *, int, int, int, int &, int &);

    void mergeSortRecursive(int *, int, int, int &, int &);

    int partition(int *, int, int, int &, int &);

    void quickSortRecursive(int *, int, int, int &, int &);

    void hybridSortRecursive(int *, int, int, int &, int &);
};


#endif //SORTING_H

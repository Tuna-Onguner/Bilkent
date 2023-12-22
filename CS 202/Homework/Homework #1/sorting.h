/*
 * Title: Sorting and Algorithm Efficiency
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 1
 * Description: Header file for sorting.cpp,
 *              contains the function prototypes of the sorting algorithms and the helper functions.
 */

#ifndef SORTING_H
#define SORTING_H


#include <cstdio> // used for printf function while displaying arrays and outputting the results
#include <cstdlib> // used for rand function while creating random arrays

// Global function prototypes
void displayArray(const int *arr, const int size);

void createRandomArrays(int *&arr1, int *&arr2, int *&arr3, const int size);

void createAscendingArrays(int *&arr1, int *&arr2, int *&arr3, const int size);

void createDescendingArrays(int *&arr1, int *&arr2, int *&arr3, const int size);


// Sorting class
class Sorting {
public:
    void bubbleSort(int *arr, const int size, long &compCount, long &moveCount);

    void mergeSort(int *arr, const int size, long &compCount, long &moveCount);

    void quickSort(int *arr, const int size, long &compCount, long &moveCount);

private:
    void swap(int &a, int &b);

    void mergeSort(int *arr, const int left, const int right, long &compCount, long &moveCount);

    void merge(int *arr, const int left, const int middle, const int right, long &compCount, long &moveCount);

    void quickSort(int *arr, const int low, const int high, long &compCount, long &moveCount);

    int partition(int *arr, const int low, const int high, long &compCount, long &moveCount);
};


#endif // SORTING_H

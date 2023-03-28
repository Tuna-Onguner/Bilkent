/**
 * Title: Algorithm analysis & sorting
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 2
 * Homework: 1
 * Description: the implementations of the sorting algorithms insertion sort, bubble sort,
 *              merge sort, quick sort, and hybrid sort with comparison and data move counts
 */

#include "sorting.h"

void Sorting::insertionSort(int *arr, const int size, int &compCount, int &moveCount) {
    for (int i = 1; i < size; i++) {
        int temp = arr[i];
        moveCount++; // Data move: arr[i] to temp
        int j = i - 1;
        while (arr[j] > temp && j >= 0) {
            compCount++; // Comparison: arr[j] > temp (when true)
            arr[j + 1] = arr[j];
            moveCount++; // Data move: arr[j] to arr[j+1]
            j--;
        }
        compCount++; // Comparison: arr[j] > temp (when false)
        arr[j + 1] = temp;
        moveCount++; // Data move: temp to arr[j+1]
    }
}

void Sorting::swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void Sorting::bubbleSort(int *arr, const int size, int &compCount, int &moveCount) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            compCount++; // Comparison: arr[j] > arr[j+1]
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                moveCount += 3; // Data moves: arr[j] to temp, arr[j+1] to arr[j], temp to arr[j+1]
            }
        }
    }
}

void Sorting::bubbleSort(int *arr, int start, int end, int &compCount, int &moveCount) {
    for (int i = start; i <= end; i++) {
        for (int j = start; j <= end - i - 1; j++) {
            compCount++;
            if (arr[j] > arr[j + 1]) {
                moveCount += 3; // Data moves: arr[j] to temp, arr[j+1] to arr[j], temp to arr[j+1]
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void Sorting::merge(int *arr, int left, int middle, int right, int &compCount, int &moveCount) {
    int n1 = middle - left + 1;
    int n2 = right - middle;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
        moveCount++; // Data move: arr[left+i] to L[i]
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[middle + 1 + j];
        moveCount++; // Data move: arr[middle+1+j] to R[j]
    }
    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
        compCount++; // Comparison: L[i] <= R[j]
        moveCount++; // Data move: L[i] or R[j] to arr[k]
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
        moveCount++; // Data move: L[i] to arr[k]
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        moveCount++; // Data move: R[j] to arr[k]
    }
}

void Sorting::mergeSortRecursive(int *arr, int left, int right, int &compCount, int &moveCount) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSortRecursive(arr, left, middle, compCount, moveCount);
        mergeSortRecursive(arr, middle + 1, right, compCount, moveCount);
        merge(arr, left, middle, right, compCount, moveCount);
    }
}

void Sorting::mergeSort(int *arr, const int size, int &compCount, int &moveCount) {
    mergeSortRecursive(arr, 0, (size - 1), compCount, moveCount);
}

int Sorting::partition(int *arr, int low, int high, int &compCount, int &moveCount) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
            moveCount += 3; // Data move: arr[i] to temp, arr[j] to arr[i], temp to arr[j]
        }
        compCount++; // Comparison: arr[j] <= pivot
    }
    swap(&arr[i + 1], &arr[high]);
    moveCount += 3; // Data moves: arr[i+1] to temp, arr[high] to arr[i+1], temp to arr[high]
    return i + 1;
}

void Sorting::quickSortRecursive(int *arr, int low, int high, int &compCount, int &moveCount) {
    if (low < high) {
        int pi = partition(arr, low, high, compCount, moveCount);
        quickSortRecursive(arr, low, pi - 1, compCount, moveCount);
        quickSortRecursive(arr, pi + 1, high, compCount, moveCount);
    }
}

void Sorting::quickSort(int *arr, const int size, int &compCount, int &moveCount) {
    quickSortRecursive(arr, 0, (size - 1), compCount, moveCount);
}

void Sorting::hybridSortRecursive(int *arr, int start, int end, int &compCount, int &moveCount) {
    // The algorithm starts with the quick sort
    if (end - start + 1 <= 20) { // but when the partition size becomes less than or equal to 20
        bubbleSort(arr, start, end, compCount, moveCount); // sorts that partition with the bubble sort
    } else {
        int pivot = partition(arr, start, end, compCount, moveCount);
        hybridSortRecursive(arr, start, pivot - 1, compCount, moveCount);
        hybridSortRecursive(arr, pivot + 1, end, compCount, moveCount);
    }
}

void Sorting::hybridSort(int *arr, const int size, int &compCount, int &moveCount) {
    hybridSortRecursive(arr, 0, size - 1, compCount, moveCount);
}

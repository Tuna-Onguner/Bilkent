/*
 * Title: Sorting and Algorithm Efficiency
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 1
 * Description: The implementations of the sorting algorithms, namely bubble sort, merge sort and quick sort.
 *              The algorithms also keep track of the number of comparisons computed and data moves rendered.
 */

#include "sorting.h"

/**
 * @brief Displays the contents of an array
 * @param arr The array to be displayed
 * @param size The size of the array
 */
void displayArray(const int *arr, const int size) {
    printf("[ ");
    for (int i = 0; i < size; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("]\n");
}

/**
 * @brief Creates 3 fully random identical arrays
 * @param arr1 The first array to be created
 * @param arr2 The second array to be created
 * @param arr3 The third array to be created
 * @param size The size of the arrays
 */
void createRandomArrays(int *&arr1, int *&arr2, int *&arr3, const int size) {

    // Create the arrays
    arr1 = new int[size];
    arr2 = new int[size];
    arr3 = new int[size];

    // Fill the arrays with random numbers
    for (int i = 0; i < size; ++i) {
        const int randomInteger = rand();
        arr1[i] = randomInteger;
        arr2[i] = randomInteger;
        arr3[i] = randomInteger;
    }
}

/**
 * @brief Creates 3 identical ascending arrays
 * @param arr1 The first array to be created
 * @param arr2 The second array to be created
 * @param arr3 The third array to be created
 * @param size The size of the arrays
 */
void createAscendingArrays(int *&arr1, int *&arr2, int *&arr3, const int size) {
    // Create the arrays
    arr1 = new int[size];
    arr2 = new int[size];
    arr3 = new int[size];

    // Fill the arrays with ascending numbers
    for (int i = 0; i < size; ++i) {
        arr1[i] = i;
        arr2[i] = i;
        arr3[i] = i;
    }
}

/**
 * @brief Creates 3 identical descending arrays
 * @param arr1 The first array to be created
 * @param arr2 The second array to be created
 * @param arr3 The third array to be created
 * @param size The size of the arrays
 */
void createDescendingArrays(int *&arr1, int *&arr2, int *&arr3, const int size) {
    // Create the arrays
    arr1 = new int[size];
    arr2 = new int[size];
    arr3 = new int[size];

    // Fill the arrays with descending numbers
    for (int i = 0; i < size; ++i) {
        arr1[i] = size - i;
        arr2[i] = size - i;
        arr3[i] = size - i;
    }
}

/**
 * @brief Swaps two integers
 * @param a First integer to be swapped
 * @param b Second integer to be swapped
 * @note The function is used in bubble sort and quick sort
 */
void Sorting::swap(int &a, int &b) {
    const int temp = a;
    a = b;
    b = temp;
}

/**
 * @brief Bubble sort algorithm
 * @param arr The array to be sorted
 * @param size The size of the array
 * @param compCount Reference to keep track of the number of comparisons computed
 * @param moveCount Reference to keep track of the number of data moves rendered
 */
void Sorting::bubbleSort(int *arr, const int size, long &compCount, long &moveCount) {
    bool sorted = false;
    for (int pass = 1; pass < size && !sorted; ++pass) {
        sorted = true; // Assume sorted
        for (int index = 0; index < size - pass; ++index) {
            const int nextIndex = index + 1;
            compCount += 1; // Comparisons + 1: arr[index] > arr[nextIndex]
            if (arr[index] > arr[nextIndex]) {
                swap(arr[index], arr[nextIndex]);
                moveCount += 3; // Data Moves + 3: Swapping costs 3 data moves
                sorted = false; // Signal exchange: Not sorted yet
            }
        }
    }
}

/**
 * @brief Merges two sub-arrays of the array
 * @param arr The array to be sorted
 * @param left The starting index of the first sub-array
 * @param middle The ending index of the first sub-array
 * @param right The ending index of the second sub-array
 * @param compCount Reference to keep track of the number of comparisons computed
 * @param moveCount Reference to keep track of the number of data moves rendered
 * @note The function is used in merge sort
 */
void Sorting::merge(int *arr, const int left, const int middle, const int right, long &compCount, long &moveCount) {

    const int maxTempArraySize = right + 1; // maximum size of temporary array
    int *tempArray = new int[maxTempArraySize]; // temporary array

    int leftFirst = left; // beginning of first subarray
    int rightFirst = middle; // end of first subarray
    int leftSecond = middle + 1; // beginning of second subarray
    int rightSecond = right; // end of second subarray
    int nextIndex = leftFirst; // next available location in tempArray

    for (; (leftFirst <= rightFirst) && (leftSecond <= rightSecond); ++nextIndex) {
        compCount += 1; // Comparisons + 1: arr[leftFirst] < arr[leftSecond]
        if (arr[leftFirst] < arr[leftSecond]) {
            tempArray[nextIndex] = arr[leftFirst];
            ++leftFirst;
        } else {
            tempArray[nextIndex] = arr[leftSecond];
            ++leftSecond;
        }
        moveCount += 1; // Data Moves + 1: tempArray[nextIndex] = (arr[leftFirst] or arr[leftSecond])
    }

    for (; leftFirst <= rightFirst; ++leftFirst, ++nextIndex) {
        tempArray[nextIndex] = arr[leftFirst];
        moveCount += 1; // Data Moves + 1: tempArray[nextIndex] = arr[leftFirst]
    }

    for (; leftSecond <= rightSecond; ++leftSecond, ++nextIndex) {
        tempArray[nextIndex] = arr[leftSecond];
        moveCount += 1; // Data Moves + 1: tempArray[nextIndex] = arr[leftSecond]
    }

    for (nextIndex = left; nextIndex <= right; ++nextIndex) {
        arr[nextIndex] = tempArray[nextIndex];
        moveCount += 1; // Data Moves + 1: arr[nextIndex] = tempArray[nextIndex]
    }

    delete[] tempArray; // free temporary array
}

/**
 * @brief Recursive merge sort algorithm
 * @param arr The array to be sorted
 * @param left The starting index of the sub-array
 * @param right The ending index of the sub-array
 * @param compCount Reference to keep track of the number of comparisons computed
 * @param moveCount Reference to keep track of the number of data moves rendered
 * @note This function is called by the helper function, and then calls itself recursively
 */
void Sorting::mergeSort(int *arr, const int left, const int right, long &compCount, long &moveCount) {
    if (left < right) { // Continue if there are more than one element
        const int middle = (right + left) / 2; // Calculate the middle index
        mergeSort(arr, left, middle, compCount, moveCount); // Sort the first and second halves
        mergeSort(arr, (middle + 1), right, compCount, moveCount); // Sort the third and fourth halves
        merge(arr, left, middle, right, compCount, moveCount); // Merge the two sub-arrays
    }
}

/**
 * @brief Helper function calling recursive merge sort
 * @param arr The array to be sorted
 * @param size The size of the array
 * @param compCount Reference to keep track of the number of comparisons computed
 * @param moveCount Reference to keep track of the number of data moves rendered
 */
void Sorting::mergeSort(int *arr, const int size, long &compCount, long &moveCount) {
    mergeSort(arr, 0, (size - 1), compCount, moveCount); // Call recursive merge sort
}

/**
 * @brief Partitions the array by choosing a pivot and placing it in its correct position
 * @param arr The array to be sorted
 * @param low The starting index of the sub-array
 * @param high The ending index of the sub-array
 * @param compCount Reference to keep track of the number of comparisons computed
 * @param moveCount Reference to keep track of the number of data moves rendered
 * @return The index of the pivot
 * @note The pivot is chosen as the very last element
 * @note This function is used in quick sort
 */
int Sorting::partition(int *arr, const int low, const int high, long &compCount, long &moveCount) {
    const int pivot = arr[low]; // The pivot is chosen as the very first element of the sub-array
    moveCount += 1; // Data moves + 1: arr[low] -> pivot
    int i = low; // First unknown element
    for (int j = low + 1; j <= high; ++j) {
        compCount += 1; // Comparisons + 1: arr[j] <= pivot
        if (arr[j] <= pivot) {
            ++i;
            swap(arr[i], arr[j]); // Swap arr[i] <-> arr[j]
            moveCount += 3; // Data moves + 3: Swapping costs 3 data moves
        }
    }
    swap(arr[i], arr[low]); // Swap arr[i] <-> arr[low]
    moveCount += 3; // Data moves + 3: Swapping costs 3 data moves
    return i; // return the index of the pivot
}

/**
 * @brief Recursive quick sort algorithm
 * @param arr The array to be sorted
 * @param low The starting index of the sub-array
 * @param high The ending index of the sub-array
 * @param compCount Reference to keep track of the number of comparisons computed
 * @param moveCount Reference to keep track of the number of data moves rendered
 * @note This function is called by the helper function, and then calls itself recursively
 */
void Sorting::quickSort(int *arr, const int low, const int high, long &compCount, long &moveCount) {
    if (low < high) { // Continue if there are more than one element
        const int pivot = partition(arr, low, high, compCount, moveCount); // Choose a pivot
        quickSort(arr, low, (pivot - 1), compCount, moveCount); // Sort the left sub-array
        quickSort(arr, (pivot + 1), high, compCount, moveCount); // Sort the right sub-array
    }
}

/**
 * @brief Helper function calling recursive quick sort
 * @param arr The array to be sorted
 * @param size The size of the array
 * @param compCount Reference to keep track of the number of comparisons computed
 * @param moveCount Reference to keep track of the number of data moves rendered
 */
void Sorting::quickSort(int *arr, const int size, long &compCount, long &moveCount) {
    quickSort(arr, 0, (size - 1), compCount, moveCount); // Call recursive quick sort
}

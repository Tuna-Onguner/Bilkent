/**
 * Title: Algorithm analysis & sorting
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 2
 * Homework: 1
 * Description: main to test sorting algorithms implemented in sorting.cpp
 */

#include <ctime> // for runtime computations
#include <math.h> // for log2(x) calculations
#include <cstdlib> // for randomly generated integers
#include <iostream> // for outputs, strings, and other required components
#include "sorting.h"

int main() {

    Sorting sort;
    const int arraySizes[] = {1000, 5000, 10000, 20000};
    int k = 0, j = 0, arraySize, compCount = 0, moveCount = 0, *arr1, *arr2, *arr3, *arr4, *arr5;
    const std::string prompts[] = {"Part 2-b-1: Performance analysis of random integers array",
                                   "Part 2-b-2: Performance analysis of partially ascending integers array",
                                   "Part 2-b-3: Performance analysis of partially descending integers array"};

    while (j <= 2) {

        printf("\n%74s\n", prompts[j].c_str());

        while (k <= 3) {

            arraySize = arraySizes[k];

            arr1 = new int[arraySize];
            arr2 = new int[arraySize];
            arr3 = new int[arraySize];
            arr4 = new int[arraySize];
            arr5 = new int[arraySize];

            if (j == 0) {
                for (int i = 0; i < arraySize; i++) {
                    int randomValue = rand();
                    arr1[i] = randomValue;
                    arr2[i] = randomValue;
                    arr3[i] = randomValue;
                    arr4[i] = randomValue;
                    arr5[i] = randomValue;
                }
            } else if (j == 1) {
                const int n = log2(arraySize);
                for (int i = 0, m = n, max = 100, min = 0; i < arraySize; i++, m--) {
                    int randomValue = rand() % (max - min + 1) + min;
                    arr1[i] = randomValue;
                    arr2[i] = randomValue;
                    arr3[i] = randomValue;
                    arr4[i] = randomValue;
                    arr5[i] = randomValue;
                    if (m <= 0) {
                        min = max;
                        max = max + 100;
                        m = n;
                    }
                }
            } else {
                const int n = log2(arraySize);
                for (int i = arraySize - 1, m = n, max = 100, min = 0; i >= 0; i--, m--) {
                    int randomValue = rand() % (max - min + 1) + min;
                    arr1[i] = randomValue;
                    arr2[i] = randomValue;
                    arr3[i] = randomValue;
                    arr4[i] = randomValue;
                    arr5[i] = randomValue;
                    if (m <= 0) {
                        min = max;
                        max = max + 100;
                        m = n;
                    }
                }
            }

            printf("--------------------------------------------------------------------------\n"
                   "%32s %20s %20s\nArray Size: %d\n", "Elapsed time", "Comp. count", "Move count", arraySize);

            clock_t clockTime;
            double elapsedTime;

            // Insertion sort
            clockTime = clock();
            sort.insertionSort(arr1, arraySize, compCount, moveCount);
            elapsedTime = 1000 * double(clock() - clockTime) / CLOCKS_PER_SEC;
            printf("Insertion sort: %14.3fms %20d %20d\n", elapsedTime, compCount, moveCount);

            compCount = 0;
            moveCount = 0;

            // Bubble sort
            clockTime = clock();
            sort.bubbleSort(arr2, arraySize, compCount, moveCount);
            elapsedTime = 1000 * double(clock() - clockTime) / CLOCKS_PER_SEC;
            printf("Bubble sort: %17.3fms %20d %20d\n", elapsedTime, compCount, moveCount);

            compCount = 0;
            moveCount = 0;

            // Merge sort
            clockTime = clock();
            sort.mergeSort(arr3, arraySize, compCount, moveCount);
            elapsedTime = 1000 * double(clock() - clockTime) / CLOCKS_PER_SEC;
            printf("Merge sort: %18.3fms %20d %20d\n", elapsedTime, compCount, moveCount);

            compCount = 0;
            moveCount = 0;

            // Quick sort
            clockTime = clock();
            sort.quickSort(arr4, arraySize, compCount, moveCount);
            elapsedTime = 1000 * double(clock() - clockTime) / CLOCKS_PER_SEC;
            printf("Quick sort: %18.3fms %20d %20d\n", elapsedTime, compCount, moveCount);

            compCount = 0;
            moveCount = 0;

            // Hybrid sort
            clockTime = clock();
            sort.hybridSort(arr5, arraySize, compCount, moveCount);
            elapsedTime = 1000 * double(clock() - clockTime) / CLOCKS_PER_SEC;
            printf("Hybrid sort: %17.3fms %20d %20d\n\n", elapsedTime, compCount, moveCount);

            compCount = 0;
            moveCount = 0;

            delete[] arr1;
            arr1 = NULL;
            delete[] arr2;
            arr2 = NULL;
            delete[] arr3;
            arr3 = NULL;
            delete[] arr4;
            arr4 = NULL;
            delete[] arr5;
            arr5 = NULL;

            k++;
        }

        k = 0;
        j++;
    }

    return 0;
}

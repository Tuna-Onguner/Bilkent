/*
 * Title: Sorting and Algorithm Efficiency
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 1
 * Description: main.cpp, the main file of the project, contains the main function.
 */

#include <ctime> // to measure the execution times of the sorting algorithms
#include <string> // to use strings in the prompts for output purposes
#include "sorting.h"

void performanceAnalysis();

int main() {

    Sorting sorting; // create an instance of the Sorting class to use its sorting algorithms

    // Part c:
    printf("\033[101m\033[1m\033[97m %-79s \033[0m\n", "Part c: Testing the sorting algorithms");
    for (int i = 0; i < 3; ++i) {
        long compCount = 0, moveCount = 0;
        int array[] = {10, 5, 9, 16, 17, 7, 4, 12, 19, 1, 15, 18, 3, 11, 13, 6};
        const int size = sizeof(array) / sizeof(array[0]);
        if (i == 0) {
            sorting.bubbleSort(array, size, compCount, moveCount);
            printf("\033[36m1. Bubble Sort: \033[0m");
            displayArray(array, size);
            printf("\033[94mComp Count:\033[0m %3ld\n\033[94mMove Count:\033[0m %3ld\n", compCount, moveCount);
        } else if (i == 1) {
            sorting.mergeSort(array, size, compCount, moveCount);
            printf("\n\033[36m2. Merge  Sort: \033[0m");
            displayArray(array, size);
            printf("\033[94mComp Count:\033[0m %3ld\n\033[94mMove Count:\033[0m %3ld\n", compCount, moveCount);
        } else {
            sorting.quickSort(array, size, compCount, moveCount);
            printf("\n\033[36m3. Quick  Sort: \033[0m");
            displayArray(array, size);
            printf("\033[94mComp Count:\033[0m %3ld\n\033[94mMove Count:\033[0m %3ld\n", compCount, moveCount);
        }
    }

    // Part d:
    printf("\n\n\033[101m\033[1m\033[97m %-79s \033[0m", "Part d: Performance analysis of the sorting algorithms");
    performanceAnalysis();

    return 0;
}

void performanceAnalysis() {

    Sorting sorting; // create an instance of the Sorting class to use its sorting algorithms

    const int arraySizesUnderTest[] = {4000, 8000, 12000, 16000, 20000, 24000, 28000, 32000, 36000, 40000};
    const std::string outputPrompts[] = {"1. Performance analysis of random integers array",
                                         "2. Performance analysis of ascending integers array",
                                         "3. Performance analysis of descending integers array"};

    for (int j = 0; j <= 2; ++j) {

        printf("\n\033[32m---------------------------------------------------------------------------------\033[0m"
               "\n\033[1m\033[3m\033[32m%63s\033[0m\n", outputPrompts[j].c_str()); // Print the output prompt

        int **arrays4BblSort = new int *[10];
        int **arrays4MrgSort = new int *[10];
        int **arrays4QckSort = new int *[10];

        switch (j) {
            case 0:
                for (int i = 0; i < 10; i++) {
                    createRandomArrays(arrays4BblSort[i], arrays4MrgSort[i], arrays4QckSort[i],
                                       arraySizesUnderTest[i]);
                }
                break;
            case 1:
                for (int i = 0; i < 10; i++) {
                    createAscendingArrays(arrays4BblSort[i], arrays4MrgSort[i], arrays4QckSort[i],
                                          arraySizesUnderTest[i]);
                }
                break;
            case 2:
                for (int i = 0; i < 10; i++) {
                    createDescendingArrays(arrays4BblSort[i], arrays4MrgSort[i], arrays4QckSort[i],
                                           arraySizesUnderTest[i]);
                }
                break;
            default:
                exit(EXIT_FAILURE); // Something went wrong
        }

        // Bubble sort
        printf("\033[95m---------------------------------------------------------------------------------\033[0m\n"
               "\033[95mAnalysis of Bubble Sort\033[0m\n%s %33s %37s %37s\n", "\033[94m\033[4mArray Size\033[0m",
               "\033[94m\033[4mElapsed Time\033[0m", "\033[94m\033[4mComp. Count\033[0m",
               "\033[94m\033[4mMove Count\033[0m");
        for (int i = 0; i < 10; i++) {
            long compCount = 0, moveCount = 0;
            const int currentArraySize = arraySizesUnderTest[i];
            const clock_t clockTime = clock();
            sorting.bubbleSort(arrays4BblSort[i], currentArraySize, compCount, moveCount);
            const double elapsedTime = 1000 * double(clock() - clockTime) / CLOCKS_PER_SEC;
            printf("%10d %18.3f\033[36mms\033[0m %24ld %24ld\n", currentArraySize, elapsedTime, compCount, moveCount);
        }

        // Merge sort
        printf("\033[95m---------------------------------------------------------------------------------\033[0m\n"
               "\033[95mAnalysis of Merge Sort\033[0m\n%s %33s %37s %37s\n", "\033[94m\033[4mArray Size\033[0m",
               "\033[94m\033[4mElapsed Time\033[0m", "\033[94m\033[4mComp. Count\033[0m",
               "\033[94m\033[4mMove Count\033[0m");
        for (int i = 0; i < 10; i++) {
            long compCount = 0, moveCount = 0;
            const int currentArraySize = arraySizesUnderTest[i];
            const clock_t clockTime = clock();
            sorting.mergeSort(arrays4MrgSort[i], currentArraySize, compCount, moveCount);
            const double elapsedTime = 1000 * double(clock() - clockTime) / CLOCKS_PER_SEC;
            printf("%10d %18.3f\033[36mms\033[0m %24ld %24ld\n", currentArraySize, elapsedTime, compCount, moveCount);
        }

        // Quick sort
        printf("\033[95m---------------------------------------------------------------------------------\033[0m\n"
               "\033[95mAnalysis of Quick Sort\033[0m\n%s %33s %37s %37s\n", "\033[94m\033[4mArray Size\033[0m",
               "\033[94m\033[4mElapsed Time\033[0m", "\033[94m\033[4mComp. Count\033[0m",
               "\033[94m\033[4mMove Count\033[0m");
        for (int i = 0; i < 10; i++) {
            long compCount = 0, moveCount = 0;
            const int currentArraySize = arraySizesUnderTest[i];
            const clock_t clockTime = clock();
            sorting.quickSort(arrays4QckSort[i], currentArraySize, compCount, moveCount);
            const double elapsedTime = 1000 * double(clock() - clockTime) / CLOCKS_PER_SEC;
            printf("%10d %18.3f\033[36mms\033[0m %24ld %24ld\n", currentArraySize, elapsedTime, compCount, moveCount);
        }

        // Free the memory allocated for the arrays
        for (int i = 0; i < 10; i++) {
            delete[] arrays4BblSort[i];
            delete[] arrays4MrgSort[i];
            delete[] arrays4QckSort[i];
        }

        delete[] arrays4BblSort;
        delete[] arrays4MrgSort;
        delete[] arrays4QckSort;
    }
}

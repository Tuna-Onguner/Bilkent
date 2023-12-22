/*
 * Title: Heaps and AVL Tree
 * Author: Deniz Tuna ONGUNER
 * ID: 22001788
 * Section: 1
 * Assignment: 3
 * Description: Implementation of heapSort function, and main function to test heapSort function.
 */

#include <fstream> //To read and write files
#include <iostream> //To print onto the console

#include "heap.h"

void swap(int &i1, int &i2) {
    const int temp = i1;
    i1 = i2;
    i2 = temp;
}

void heapSort(int *arr, const int size, int &comparisonCount) {

    comparisonCount = 0;

    for (int index = (size / 2) - 1; index >= 0; index--) {
        heapRebuild(arr, index, size, comparisonCount);
    }

    for (int last = size - 1; last > 0; last--) {
        swap(arr[0], arr[last]);
        heapRebuild(arr, 0, last, comparisonCount);
    }
}

void heapSort2(int *arr, const int size, int &comparisonCount) {

    Heap heap; // Create a heap
    for (int i = 0; i < size; ++i) {
        heap.insert(arr[i]); // Insert all the elements of the array into the heap
    }

    for (int i = size - 1; i >= 0; --i) {
        arr[i] = heap.popMaximum(); // Pop the maximum element of the heap and put it into the array
    }

    comparisonCount = heap.getComparisonCount(); // Get the number of comparisons made by the heap
}

int main(int argc, char **argv) {

    // Check the number of parameters
    if (argc != 3) {
        std::cerr << "Usage: " << "<input_file>" << " <output_file>" << std::endl;
        return 1;
    }

    // Open the input file
    const std::string inputFileName = argv[1];
    std::ifstream inputFile(inputFileName.c_str());

    // Check if the input file is opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the file \"" << inputFileName << "\"." << std::endl;
        return 1;
    }

    // Read the input file
    int *array, size = 0;
    for (int num; inputFile >> num; ++size) {
        if (size <= 0) {
            array = new int[size + 1];
            array[size] = num;
        } else {
            const int *temp = array;
            array = new int[size + 1];
            for (int i = 0; i < size; ++i) {
                array[i] = temp[i];
            }
            array[size] = num;
            delete[] temp;
        }
    }

    // Close the input file
    inputFile.close();

    // Sort the array
    int comparisonCount;
    //heapSort(array, size, comparisonCount);
    heapSort2(array, size, comparisonCount);

    // Open the output file
    const std::string outputFileName = argv[2];
    std::ofstream outputFile(outputFileName.c_str());

    // Check if the output file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open the file \"" << outputFileName << "\"." << std::endl;
        return 1;
    }

    // Write the sorted array to the output file
    for (int i = 0; i < size; ++i) {
        outputFile << array[i] << std::endl;
    }

    // Write the array size to the output file
    std::cout << "Size of the sorted array (n): " << size;

    // Write the number of comparisons made by heapsort to the output file
    std::cout << std::endl << "Number of comparisons made by heapsort: " << comparisonCount << std::endl;

    // Close the output file
    outputFile.close();

    // Delete the array
    delete[] array;

    return 0;
}

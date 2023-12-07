/*
 * Bilkent University
 * Semester of Fall 2023
 *
 * Department of Computer Engineering
 *
 * CS342 Operating Systems
 * Project #1: Concurrent Processes, IPC, and Threads
 *
 * Section 1
 * Instructor: İbrahim Körpeoğlu
 *
 * Authors: Deniz Tuna Onguner (22001788) & Alper Göçmen (22002948)
 *
 * version 1.0
 * Date: 20 October 2023, Friday
 *
 * Description: This is the file for the program `primeT` which is to find the prime numbers in a given
 *              file and write them onto a new file via threads.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define MAX_THREADS 20

// Mutex for thread synchronization
pthread_mutex_t mutex;

// Define a struct for prime numbers and their frequencies
struct PrimeNode {
    int prime;
    int frequency;
    struct PrimeNode* next;
};

// Global linked list for prime numbers
struct PrimeNode* globalPrimeList = NULL;

// Function to check if an integer is prime using the Sieve of Eratosthenes
int isPrime(int n) {
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return 0;
    }

    // Check for primality using 6k +/- 1 rule
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }

    return 1;
}

// Function to add a prime number to the linked list
struct PrimeNode* addPrimeToList(struct PrimeNode* head, int prime) {
    struct PrimeNode* newNode = (struct PrimeNode*)malloc(sizeof(struct PrimeNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->prime = prime;
    newNode->frequency = 1;
    newNode->next = head;
    return newNode;
}

// Function to process an intermediate file
void* processIntermediateFile(void* arg) {
    int fileIndex = *((int*)arg);

    // Initialize a local linked list for prime numbers
    struct PrimeNode* localHead = NULL;

    // Open the intermediate file for reading
    char intermediate_filename[64];
    snprintf(intermediate_filename, sizeof(intermediate_filename), "intermediate%d.txt", fileIndex);
    FILE* intermediate_file = fopen(intermediate_filename, "r");
    if (intermediate_file == NULL) {
        perror("Error opening intermediate file");
        exit(EXIT_FAILURE);
    }

    // Read integers from the intermediate file, one at a time
    int num;
    while (fscanf(intermediate_file, "%d", &num) == 1) {
        if (isPrime(num)) {
            // If the number is prime, add it to the local linked list (localHead)
            localHead = addPrimeToList(localHead, num);
        }
    }

    // Close the intermediate file
    fclose(intermediate_file);

    // Lock the mutex before updating the global data structure
    pthread_mutex_lock(&mutex);

    // Merge local linked list into the global data structure
    while (localHead != NULL) {
    struct PrimeNode* current = globalPrimeList;
    struct PrimeNode* prev = NULL;
    int found = 0;

        while (current != NULL) {
            if (current->prime == localHead->prime) {
                current->frequency += localHead->frequency;
                found = 1;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (!found) {
            // Element not found in globalPrimeList, so add it.
            struct PrimeNode* newNode = (struct PrimeNode*)malloc(sizeof(struct PrimeNode));
            newNode->prime = localHead->prime;
            newNode->frequency = localHead->frequency;
            newNode->next = NULL;

            if (prev == NULL) {
                newNode->next = globalPrimeList;
                globalPrimeList = newNode;
            } else {
                newNode->next = prev->next;
                prev->next = newNode;
            }
        }

        localHead = localHead->next;
    }

    // Unlock the mutex after updating the global data structure
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    int num_threads = 5;  // Default value
    char* input_file = NULL;
    char* output_file = NULL;
    pthread_t threads[MAX_THREADS];
    int fileIndexes[MAX_THREADS];

    // Process Command-Line Arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:i:o:")) != -1) {
        switch (opt) {
            case 'n':
                // Parse the value of the -n option
                char* endptr;
                long num_threads_long;

                // Use strtol to convert optarg to a long integer
                num_threads_long = strtol(optarg, &endptr, 10);

                // Check for conversion errors and validity of the argument
                if (errno == ERANGE || *endptr != '\0' || num_threads_long < 1 || num_threads_long > MAX_THREADS) {
                    fprintf(stderr, "Invalid value for -n option. Must be an integer between 1 and %d.\n", MAX_THREADS);
                    exit(EXIT_FAILURE);
                }

                // Convert the parsed long value to an integer
                num_threads = (int)num_threads_long;
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            default:
                fprintf(stderr, "Usage: primeT -n N -i INFILE -o OUTFILE\n");
                exit(EXIT_FAILURE);
        }
    }

    // Validate the number of threads
    if (num_threads < 1 || num_threads > MAX_THREADS) {
        fprintf(stderr, "Number of threads must be between 1 and %d.\n", MAX_THREADS);
        exit(EXIT_FAILURE);
    }

    // Initialize the mutex for thread synchronization
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    // Read integers from the input file and split them into intermediate files
    FILE* input_file_ptr = fopen(input_file, "r");
    if (input_file_ptr == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    int fileIndex = 1;
    int num;
    char intermediate_filename[64];
    FILE* intermediate_file;

    while (fscanf(input_file_ptr, "%d", &num) == 1) {
        snprintf(intermediate_filename, sizeof(intermediate_filename), "intermediate%d.txt", fileIndex);
        intermediate_file = fopen(intermediate_filename, "a");

        if (intermediate_file == NULL) {
            perror("Error opening intermediate file");
            exit(EXIT_FAILURE);
        }

        fprintf(intermediate_file, "%d\n", num);
        fclose(intermediate_file);

        fileIndex = (fileIndex % num_threads) + 1;
    }

    fclose(input_file_ptr);

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        fileIndexes[i] = i + 1;
        if (pthread_create(&threads[i], NULL, processIntermediateFile, &fileIndexes[i]) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Thread join failed");
            exit(EXIT_FAILURE);
        }
    }

    // Open the output file for writing
    FILE* output_file_ptr = fopen(output_file, "w");
    if (output_file_ptr == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Iterate through the merged data structure (globalPrimeList) and write prime numbers to the output file
    struct PrimeNode* current = globalPrimeList;
    while (current != NULL) {
        for (int i = 0; i < current->frequency; i++) {
            fprintf(output_file_ptr, "%d\n", current->prime);
        }
        current = current->next;
    }

    // Close the output file
    fclose(output_file_ptr);

    // Remove intermediate files
    
     for (int i = 1; i <= num_threads; i++) {
        snprintf(intermediate_filename, sizeof(intermediate_filename), "intermediate%d.txt", i);
        if (remove(intermediate_filename) != 0) {
            perror("Error removing intermediate file");
        }
    }

    // Clean up the mutex
    pthread_mutex_destroy(&mutex);

    // Free memory allocated for the global linked list
    while (globalPrimeList != NULL) {
        struct PrimeNode* temp = globalPrimeList;
        globalPrimeList = globalPrimeList->next;
        free(temp);
    }

    return 0;
}
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
 * Description: This is the file for the program `primeP` which is to find the prime numbers in a given
 *              file and write them onto a new file via linux posix message queues and multiprocessing.
 */

#include <wait.h> // For waitpid

#include <stdio.h> // For fprintf, snprintf, fopen, and fclose
#include <errno.h> // For perror
#include <fcntl.h> // For O_CREAT, O_RDWR, and O_CLOEXEC

#include <stdlib.h> // For strtol
#include <string.h> // For memcpy
#include <unistd.h> // For getopt
#include <mqueue.h> // For message queues
#include <stdint.h> // For int64_t

#include <inttypes.h> // For PRId64

#define POSIX_MQ_NAME "/primes_mqueue"

// Function to check if a number is prime or not
int isPrime(const int64_t NUMBER) {
    if (NUMBER <= 1) {
        return 0; // 0, 1, and negative numbers are definitely NOT prime
    }

    if (NUMBER <= 3) {
        return 1; // 2 and 3 are indeed prime
    }

    if ((NUMBER % 2 == 0) || (NUMBER % 3 == 0)) {
        return 0; // Multiples of 2 or 3 are NOT prime
    }

    // Check prime by trying divisors up to the square root of the argument/NUMBER
    for (int64_t i = 5; i * i <= NUMBER; i += 6) {
        if ((NUMBER % i == 0) || (NUMBER % (i + 2) == 0)) {
            return 0;
        }
    }

    return 1; // Then, prime
}

// Function to create intermediate files
void createIntermediateFiles(char *inputFileName, const int N) {
    FILE *inputFile = fopen(inputFileName, "r"), *intermediateFiles[N]; // Open the input file
    if (inputFile == NULL) {
        perror("input file open failed");
        exit(EXIT_FAILURE);
    }

    char intermediateFilesNames[N][30];
    for (int i = 0; i < N; i++) {
        sprintf(intermediateFilesNames[i], "inter_file_%d.txt", i); // Create the intermediate file names
        intermediateFiles[i] = fopen(intermediateFilesNames[i], "w"); // Open the intermediate files
    }

    int64_t number;
    int fileIndex = 0;
    while (fscanf(inputFile, "%" PRId64, &number) == 1) { // Read the input file line by line
        fprintf(intermediateFiles[fileIndex], "%" PRId64 "\n", number); // Write the numbers to the intermediate files
        fileIndex = (fileIndex + 1) % N; // Increment the file index in a circular manner
    }

    fclose(inputFile); // Close the input file
    for (int i = 0; i < N; i++) {
        fclose(intermediateFiles[i]); // Close the intermediate files
    }
}

int main(int argc, char **argv) {
    int opt;

    // Command line parameters
    int M = 3; // 3 is the default value for M
    int N = 5; // 5 is the default value for N
    char *inputFileName = NULL; // No default input file name
    char *outputFileName = "primesP.txt"; // primesP.txt is the default output file name

    // Parse command line parameters
    char *end;
    while ((opt = getopt(argc, argv, "n:m:i:o:")) != -1) {
        switch (opt) {
            case 'n': {
                const int N_CANDIDATE = (int) strtol(optarg, &end, 10);
                if (*end == '\0' && N_CANDIDATE > 0 && N_CANDIDATE <= 20) { // Check if N is valid
                    N = N_CANDIDATE;
                    break;
                }
                fprintf(stderr, "The value of N (-n) must fall within the range of 1 to 20.\n"
                                "%d is not a valid value for N.\n", N_CANDIDATE); // Print error message
                exit(EXIT_FAILURE);
            }
            case 'm': {
                const int M_CANDIDATE = (int) strtol(optarg, &end, 10);
                if (*end == '\0' && M_CANDIDATE > 0 && M_CANDIDATE <= 20) { // Check if M is valid
                    M = M_CANDIDATE;
                    break;
                }
                fprintf(stderr, "The value of M (-m) must fall within the range of 1 to 20.\n"
                                "%d is not a valid value for M.\n", M_CANDIDATE); // Print error message
                exit(EXIT_FAILURE);
            }
            case 'i': {
                inputFileName = optarg; // Set the input file name
                break;
            }
            case 'o': {
                outputFileName = optarg; // Set the output file name
                break;
            }
            default: {
                fprintf(stderr, "Usage: %s -n N -m M -i INFILE -o OUTFILE\n", argv[0]); // Invalid parameters
                exit(EXIT_FAILURE);
            }
        }
    }

    // Create and fill intermediate files
    createIntermediateFiles(inputFileName, N);

    // Create and open the message queue
    mqd_t mq;
    mq = mq_open(POSIX_MQ_NAME, O_CREAT | O_RDWR | O_CLOEXEC, 0666, NULL);
    if (mq == -1) {
        perror("mq_open failed");
        exit(EXIT_FAILURE);
    }

    // Get the maximum message size
    struct mq_attr mqAttributes;
    mq_getattr(mq, &mqAttributes);

    // Set the maximum message size
    const int DEFAULT_MAX_MSG_SIZE = mqAttributes.mq_msgsize;
    const int MSG_SIZE = M * sizeof(int64_t);

    // Create child processes
    pid_t pid; // Process ID
    for (int i = 0; i < N; i++) {
        pid = fork(); // Create a child process

        if (pid == 0) {
            // Child process (i-th)
            char fileName[30];
            snprintf(fileName, sizeof(fileName), "inter_file_%d.txt", i); // Create the intermediate file name

            // Open the intermediate file
            FILE *file = fopen(fileName, "r");
            if (file == NULL) {
                perror("intermediate file open failed (on child)");
                _exit(EXIT_FAILURE);
            }

            int currPrimeCountInMsg = 0; // Current prime count in the message
            int64_t candidateNumber, message[M];
            while (fscanf(file, "%" PRId64, &candidateNumber) == 1) { // Read the file line by line
                if (isPrime(candidateNumber)) { // If the number is prime
                    message[currPrimeCountInMsg] = candidateNumber; // Add the prime to the message
                    currPrimeCountInMsg += 1; // Increment the prime count in the message

                    if (currPrimeCountInMsg == M) { // If the message is fulled
                        char charMessage[MSG_SIZE];
                        memcpy(charMessage, message, MSG_SIZE); // Copy the message to a char array

                        if (mq_send(mq, charMessage, MSG_SIZE, 0) == -1) { // Send the message
                            perror("mq_send (on child) failed");
                            _exit(EXIT_FAILURE);
                        }

                        currPrimeCountInMsg = 0; // Reset the message counter
                    }
                }
            }

            // Send the remaining primes if any
            if (currPrimeCountInMsg > 0) {
                const int MSG_SIZE_REMAINING = currPrimeCountInMsg * sizeof(int64_t);

                char charMessage[MSG_SIZE_REMAINING]; // Create a char array for the remaining primes
                memcpy(charMessage, message, MSG_SIZE_REMAINING); // Copy the remaining primes to the char array

                if (mq_send(mq, charMessage, MSG_SIZE_REMAINING, 0) == -1) { // Send the remaining primes
                    perror("mq_send --for remaining-- (on child) failed");
                    _exit(EXIT_FAILURE);
                }
            }

            fclose(file); // Close the file
            remove(fileName); // Remove the intermediate file, since it is no longer needed

            mq_close(mq); // Close the message queue
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process reads messages
    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        perror("output file open failed");
        exit(EXIT_FAILURE);
    }

    int64_t message[M]; // Create a message array
    int status, numberOfChildProcessesRunning = N; // Number of child processes running
    while (numberOfChildProcessesRunning > 0) {
        const pid_t TERMINATED_PID = waitpid(-1, &status, WNOHANG); // Check if any child process terminated
        if (TERMINATED_PID > 0) { // If a child process terminated
            numberOfChildProcessesRunning -= 1; // Decrement the number of child processes running
        }

        mq_getattr(mq, &mqAttributes); // Check if there are no more messages to be received
        for (int k = mqAttributes.mq_curmsgs; k > 0; k = k - 1) {
            char charMessage[MSG_SIZE]; // Create a char array for the message
            const ssize_t RECEIVED = mq_receive(mq, charMessage, DEFAULT_MAX_MSG_SIZE, 0); // Receive a message

            if (RECEIVED == -1) { // Error occurred while receiving the message
                perror("mq_receive failed");
                exit(EXIT_FAILURE);
            } else { // Message received successfully
                const int RECEIVED_SIZE = (int) RECEIVED / sizeof(int64_t); // Calculate the size of the message
                memcpy(message, charMessage, RECEIVED); // Copy the message to the message array

                for (int i = 0; i < RECEIVED_SIZE; i++) { // Write the primes to the output file
                    fprintf(outputFile, "%" PRId64 "\n", ((int64_t *) message)[i]);
                }
            }
        }
    }

    // Close and unlink the message queue
    mq_close(mq);
    mq_unlink(POSIX_MQ_NAME);

    // Close the output file
    fclose(outputFile);

    return 0;
}

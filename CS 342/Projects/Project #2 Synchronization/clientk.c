/*
 * Bilkent University
 * Semester of Fall 2023
 *
 * Department of Computer Engineering
 *
 * CS342 Operating Systems
 * Project #2: Synchronization
 *
 * Section 1
 * Instructor: İbrahim Körpeoğlu
 *
 * Authors: Deniz Tuna Onguner (22001788) & Hasan Ege Tunç (22003814)
 *
 * version 1.0
 * Date: 19 November 2023
 *
 * Description: (clientk) ...
 */

#include "shareddefinitions.h"

/*
 * Char array allocation is done based on:
 * Max Value of L (value size) is 1024 specified in the project file
 * 4 bytes for the request specification
 * 8 bytes for the thread_id specification
 * 8 bytes for the key specification
 * 1 byte for operation successful(S), unsuccessful bit(U)
 * 1 byte for the null terminator
 * Summing these values we have 1045 bits.
 */

#define MAX_NUMBER_OF_THREADS 10

/*
 * Global variables for multi-threading mode
 */
pthread_cond_t condition_var[MAX_NUMBER_OF_THREADS];
pthread_mutex_t mutex_var[MAX_NUMBER_OF_THREADS];
pthread_t t_ids[MAX_NUMBER_OF_THREADS];
pthread_t FE_thread_id;
Message *replies[MAX_NUMBER_OF_THREADS];
pthread_mutex_t reply_mutex_var[MAX_NUMBER_OF_THREADS];
int is_exited[MAX_NUMBER_OF_THREADS];
FILE *input_files[MAX_NUMBER_OF_THREADS];

/*
 * Global variables for interactive mode
 */
pthread_t interactive_thread_id;
Message *interactive_reply;
pthread_mutex_t interactive_reply_mutex_var = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t interactive_reply_cond_var = PTHREAD_COND_INITIALIZER;
int is_exited_interactive = 0;

//number of initialized threads
int N;
char *client_fname, *mqname;
int client_vsize, clicount, dlevel;
mqd_t mq1;
mqd_t mq2;
//SharedData *shared_data;
/*
 * Function to process the file by a single thread
 */
void *processFile() {
    pthread_t my_t_id = pthread_self();
    // Search for the index of the current thread ID in t_ids
    int target_index = -1;
    for (int i = 0; i < N; ++i) {
        if (pthread_equal(my_t_id, t_ids[i])) {
            target_index = i;
            break;
        }
    }
    int processedNumber = 0;
    if (target_index == -1) {
        fprintf(stderr, "Error finding thread ID in t_ids\n");
        exit(EXIT_FAILURE);
    }
    char line[MAX_MESSAGE_SIZE];
    char *buffer = malloc(MAX_MESSAGE_SIZE);
    char request_type[4];
    long key;
    char *value = malloc(client_vsize);
    while (fgets(line, sizeof(line), input_files[target_index]) != NULL) {
        // Remove the trailing newline character if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        Message *msg;
        sscanf(line, "%3s %ld %s", request_type, &key, value);
        msg = createMessage(request_type, key, value, 'U', client_vsize);

        if (dlevel == 1) {
            printf("Request: %s, Key: %ld, Value: %s, Thread ID: %lu was sent\n",
                   msg->request_type, msg->key, msg->value, msg->t_id);
        }
        // Encode the message
        encodeMessage(msg, buffer, client_vsize);
        //pthread_mutex_lock(&(shared_data->server_FE_mutex_var));

        if (mq_send(mq1, buffer, MAX_MESSAGE_SIZE, 0) == -1) {
            perror("Error sending message");
            exit(EXIT_FAILURE);
        }

        decodeMessage(buffer, msg, client_vsize);
        //free the message
        free(msg->value);
        free(msg);

        //printf("Signal sent to server\n");

        //pthread_cond_signal(&(shared_data->server_FE_cond_var));
        //pthread_mutex_unlock(&(shared_data->server_FE_mutex_var));

        // Wait for the reply
        pthread_cond_wait(&condition_var[target_index], &mutex_var[target_index]);
        printf("Thread with target index %d waken up %d times\n", target_index, processedNumber);
        // Get the reply & hold the lock so that reply cannot be changed
        pthread_mutex_lock(&reply_mutex_var[target_index]);
        msg = replies[target_index];
        if (dlevel == 1)
            printf("Request: %s, Key: %ld, Value: %s, Thread ID: %d is completed with status %c\n",
                   msg->request_type, msg->key, msg->value, target_index, msg->operation_successful);
        //free the message
        free(msg->value);
        free(msg);
        pthread_mutex_unlock(&reply_mutex_var[target_index]);
        processedNumber++;
        printf("Thread with target index %d processed %d lines\n", target_index, processedNumber);
    }
    //free the value;
    free(value);
    //free the buffer
    free(buffer);
    // Set the is_exited flag to 1
    is_exited[target_index] = 1;
    printf("Thread with target index exited %d\n", target_index);
    exit(EXIT_SUCCESS);
}

void *manageThreads() {
    // Create N threads
    for (int i = 0; i < N; ++i) {
        if (pthread_create(&t_ids[i], NULL, processFile, NULL) != 0) {
            fprintf(stderr, "Error creating worker threads %d\n", i);
            exit(EXIT_FAILURE);
        }
    }
    // Main loop of the FE thread
    while (1) {
        // Check if all worker threads have terminated
        int allThreadsExited = 1;  // Assume all threads have exited

        for (int i = 0; i < N; ++i) {
            if (is_exited[i] == 0) {
                // At least one thread has not exited
                allThreadsExited = 0;
                printf("All threads exited successfully\n");
                break;
            }
        }

        if (allThreadsExited) {
            // All threads have exited, break out of the loop
            break;
        }

        //wait on a client fe condition variable
        //pthread_cond_wait(&(shared_data->client_FE_cond_var), &(shared_data->client_FE_mutex_var));

        struct mq_attr attr;
        if (mq_getattr(mq2, &attr) == -1) {
            perror("mq_getattr");
            exit(EXIT_FAILURE);
        }
        int buffer_len = attr.mq_msgsize;

        //while mq2 is not empty
        //while (attr.mq_curmsgs != 0) {

        // Receive the message
        pthread_t t_id;
        char *buffer = malloc(MAX_MESSAGE_SIZE);
        Message *msg = malloc(sizeof(Message));
        mq_getattr(mq2, &attr);
        if (attr.mq_curmsgs > 0) {
            if (mq_receive(mq2, buffer, buffer_len, 0) == -1) {
                perror("Error sending message");
                exit(EXIT_FAILURE);
            }
            printf("Message received\n");
            // Decode the message
            decodeMessage(buffer, msg, client_vsize);
            t_id = msg->t_id;

            // Search for the index of the thread whose response has arrived
            int target_index = -1;
            for (int i = 0; i < N; ++i) {
                if (pthread_equal(t_id, t_ids[i])) {
                    target_index = i;
                    break;
                }
            }
            if (target_index == -1) {
                fprintf(stderr, "Error finding thread ID in t_ids\n");
                exit(EXIT_FAILURE);
            }

            // Set the reply and wake up the thread whose reply has reached
            pthread_mutex_lock(&reply_mutex_var[target_index]);
            replies[target_index] = msg;
            pthread_mutex_unlock(&reply_mutex_var[target_index]);

            pthread_cond_signal(&condition_var[target_index]);
            if (mq_getattr(mq2, &attr) == -1) {
                perror("mq_getattr");
                exit(EXIT_FAILURE);
            }
        }
        free(buffer);

    }
    // The FE thread will exit when all worker threads have terminated
    exit(EXIT_SUCCESS);
}

void *interactiveMode() {
    char *buffer = malloc(MAX_MESSAGE_SIZE);
    char command[11];
    long int key;
    char key_str[9];
    char *value = malloc(client_vsize);
    Message *message;
    printf("In interactive thread\n");
    while (1) {

        printf("Enter command: ");
        fflush(stdout);

        if (fgets(buffer, MAX_MESSAGE_SIZE, stdin) == NULL) {
            perror("Error reading input");
            exit(EXIT_FAILURE);
        }

        buffer[strcspn(buffer, "\n")] = '\0';
        printf("Buffer is %s\n", buffer);
        // Tokenize the input to get command
        if (sscanf(buffer, "%10s", command) != 1) {
            fprintf(stderr, "Error: Failed to parse command\n");
            exit(EXIT_FAILURE);
        }

        if (strcmp(command, "QUIT") == 0) {
            printf("Inside QUIT\n");
            // Handle QUIT
            break;
        } else if (strcmp(command, "QUITSERVER") == 0) {
            printf("Inside QUITSERVER\n");
            // Handle QUITSERVER
            message = createMessage("QTS", 0, NULL, 'U', client_vsize);
            // Encode the message
            encodeMessage(message, buffer, client_vsize);
            // Send the Message
            if (mq_send(mq1, buffer, MAX_MESSAGE_SIZE, 0) == -1) {
                perror("Error sending message");
                exit(EXIT_FAILURE);
            }
            //free the message
            free(message);
            // break the loop
            break;
        } else if (strcmp(command, "DUMP") == 0) {
            // Assuming the command is DUMP
            printf("Inside DUMP\n");
            const char *spacePosition = strchr(buffer, ' ');

            if (spacePosition == NULL) {
                fprintf(stderr, "Error: DUMP command requires an output file name\n");
                exit(EXIT_FAILURE);
            }
            // Extract the entire line after the first space as the value
            //printf("Space position is %s\n", spacePosition + 1);
            strncpy(value, spacePosition + 1, client_vsize);
            value[client_vsize - 1] = '\0';  // Ensure null-termination
            //printf("Output file name is %s\n", value);
            if (value[0] == '\0') {
                fprintf(stderr, "Error: DUMP command requires a non-empty output file name\n");
                exit(EXIT_FAILURE);
            }
            message = createMessage("DMP", 0, value, 'U', client_vsize);
            encodeMessage(message, buffer, client_vsize);
            free(message->value);
            free(message);
        } else if (strcmp(command, "PUT") == 0) {
            // Assuming the command is PUT
            const char *spacePosition = strchr(buffer, ' ');
            if (sscanf(spacePosition + 1, "%s", key_str) != 1) {
                fprintf(stderr, "Error: Invalid input format for %s\n", command);
                exit(EXIT_FAILURE);
            }
            key = atol(key_str);
            if (key == 0) {
                fprintf(stderr, "Error: Invalid key\n");
                exit(EXIT_FAILURE);
            }
            printf("Key is: %ld\n", key);

            spacePosition = strchr(spacePosition + 1, ' ');
            if (spacePosition == NULL) {
                fprintf(stderr, "Error: PUT command requires a value\n");
                exit(EXIT_FAILURE);
            }
            // Extract the entire line after the second space as the value
            strncpy(value, spacePosition + 1, client_vsize);
            value[client_vsize - 1] = '\0';  // Ensure null-termination
            printf("Value is: %s\n", value);
            // Create corresponding Message structs
            message = createMessage(command, key, value, 'U', client_vsize);
            encodeMessage(message, buffer, client_vsize);
            decodeMessage(buffer, message, client_vsize);

            free(message->value);
            free(message);
        } else if (strcmp(command, "GET") == 0 || strcmp(command, "DEL") == 0) {
            // Assuming the command is GET or DEL
            const char *spacePosition = strchr(buffer, ' ');
            if (sscanf(spacePosition + 1, "%s", key_str) != 1) {
                fprintf(stderr, "Error: Invalid input format for %s\n", command);
                exit(EXIT_FAILURE);
            }
            key = atol(key_str);
            if (key == 0) {
                fprintf(stderr, "Error: Invalid key\n");
                exit(EXIT_FAILURE);
            }
            // Create corresponding Message structs
            message = createMessage(command, key, NULL, 'U', client_vsize);
            encodeMessage(message, buffer, client_vsize);
            printf("Key is: %ld\n", key);
            free(message);
        } else {
            //invalid command
            fprintf(stderr, "Error: Invalid command\n");
            exit(EXIT_SUCCESS);
        }
        if (strcmp(command, "QUIT") != 0) {
            // Send the Message
            if (mq_send(mq1, buffer, MAX_MESSAGE_SIZE, 0) == -1) {
                perror("Error sending message");
                exit(EXIT_FAILURE);
            }
        }
        // Free message and its value with messages including value
        if (strcmp(command, "PUT") == 0 || strcmp(command, "DUMP") == 0) {
            free(message->value);
        }
        free(message);

        //Wait for the reply
        pthread_cond_wait(&interactive_reply_cond_var, &interactive_reply_mutex_var);
        pthread_mutex_lock(&interactive_reply_mutex_var);
        if (dlevel == 1) {
            printf("Request: %s, Key: %ld, Value: %s, Thread ID: %lu is completed with status: %c\n",
                   interactive_reply->request_type, interactive_reply->key, interactive_reply->value,
                   interactive_reply->t_id, interactive_reply->operation_successful);
        }
        pthread_mutex_unlock(&interactive_reply_mutex_var);
    }
    if (strcmp(command, "QUITSERVER") == 0) {
        //free the message
        free(message);
    }
    free(buffer);
    free(value);
    // Set the is_exited flag to 1
    is_exited_interactive = 1;
    printf("Exiting interactive mode\n");
    exit(EXIT_SUCCESS);
}
void *manageInteractiveMode() {
    //Create the interactive thread and give argc and argv as arguments
    if (pthread_create(&interactive_thread_id, NULL, interactiveMode, NULL) != 0) {
        fprintf(stderr, "Error creating interactive thread \n");
        exit(EXIT_FAILURE);
    }
    Message *msg = malloc(sizeof(Message));
    char *buffer = malloc(MAX_MESSAGE_SIZE);
    while (is_exited_interactive == 0) {
        //while mq2 is not empty
        struct mq_attr attr;
        int buffer_len = attr.mq_msgsize;
        if (mq_getattr(mq2, &attr) == -1) {
            perror("mq_getattr");
            exit(EXIT_FAILURE);
        }
        //while (attr.mq_curmsgs != 0) {
        while (1) {
            // Receive the message
            if (mq_receive(mq2, buffer, buffer_len, 0) == -1) {
                perror("Error sending message");
                exit(EXIT_FAILURE);
            }

            // Decode the message
            decodeMessage(buffer, msg, client_vsize);

            // Print for testing purposes
            //printf("Request: %s, Key: %ld, Value: %s, Thread ID: %lu\n",
            //       msg->request_type, msg->key, msg->value, msg->t_id);

            // Set the reply and wake up the thread whose reply has reached
            pthread_mutex_lock(&interactive_reply_mutex_var);
            interactive_reply = msg;
            pthread_mutex_unlock(&interactive_reply_mutex_var);

            pthread_cond_signal(&interactive_reply_cond_var);
            if (mq_getattr(mq1, &attr) == -1) {
                perror("mq_getattr");
                exit(EXIT_FAILURE);
            }
            if (is_exited_interactive)
                break;
        }
        //wait on a client fe condition variable
        //pthread_cond_wait(&(shared_data->client_FE_cond_var), &(shared_data->client_FE_mutex_var));
    }
    //free the buffer
    free(buffer);
    free(msg->value);
    free(msg);
    // The FE thread will exit when all worker threads have terminated
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {

    /*
     * get the options and set them to the globals, so threads can see them
     * vsize => value size
     * clicount => client count
     * dlevel => debug level
     * mqname => message queue name
     * fname => file name
     */
    int opt;
    while ((opt = getopt(argc, argv, "d:f:n:s:m:")) != -1) {
        switch (opt) {
            case 'd': {
                dlevel = atoi(optarg);
                if (dlevel != 0 && dlevel != 1) {
                    fprintf(stderr, "Invalid value for `dlevel`. `dlevel` must be either 0 or 1\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'f': {
                client_fname = optarg;
                break;
            }
            case 'n': {
                clicount = atoi(optarg);
                if (clicount < 0 || clicount > 10) {
                    fprintf(stderr, "Invalid value for `clicount`. `clicount` must be between 1 and 10\n");
                    exit(EXIT_FAILURE);
                }
                N = clicount;
                break;
            }
            case 's': {
                client_vsize = atoi(optarg);
                if (client_vsize < 32 || client_vsize > 1024 || client_vsize % 32 != 0) {
                    fprintf(stderr,
                            "Invalid value for `vsize`. `vsize` must be between 32 and 1024 and a multiple of 32\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'm': {
                mqname = optarg;
                break;
            }
            default: {
                fprintf(stderr, "Usage: %s -n <clicount> -f <fname> -s <vsize> -m <mqname> -d <dlevel>, \n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    char mqname1[strlen(mqname) + 3];
    char mqname2[strlen(mqname) + 3];
    sprintf(mqname1, "/%s1", mqname);
    sprintf(mqname2, "/%s2", mqname);

    mq1 = mq_open(mqname1, O_RDWR | O_CLOEXEC, 0666, NULL);
    mq2 = mq_open(mqname2, O_RDWR | O_CLOEXEC, 0666, NULL);

    if (mq1 == -1 || mq2 == -1) {
        perror("mq_open failed");
        exit(EXIT_FAILURE);
    }

    if (clicount > 0) {
        // Determine the length of the filename
        size_t fname_len = strlen(client_fname);

        // Adjust the size of the filename array
        char filename[fname_len + 6];  // 6 for "K.txt" + null terminator

        // Initialize file pointers based on the fname option
        for (int i = 0; i < N; ++i) {

            // Use snprintf to create the filename
            snprintf(filename, sizeof(filename), "%s%d.txt", client_fname, i + 1);

            // Open the file in read mode to be used by threads
            input_files[i] = fopen(filename, "r");

            // Check for errors in file opening
            if (input_files[i] == NULL) {
                fprintf(stderr, "Error opening file: %s\n", filename);
                // Handle the error as needed
                return 1;
            }
        }

        for (int i = 0; i < N; i++) {
            pthread_cond_init(&condition_var[i], NULL);
        }

        for (int i = 0; i < N; i++) {
            pthread_mutex_init(&mutex_var[i], NULL);
        }
        // Initialize the remaining elements to NULL
        for (int i = N; i < MAX_NUMBER_OF_THREADS; ++i) {
            t_ids[i] = 0;
            input_files[i] = NULL;
            is_exited[i] = 1;
        }

        // Create the front-end thread
        if (pthread_create(&FE_thread_id, NULL, manageThreads, NULL) != 0) {
            fprintf(stderr, "Error creating FE thread \n");
            return 1;
        }

        // Wait for the FE thread to finish
        pthread_join(FE_thread_id, NULL);

        return 0;
    } else {
        printf("Interactive mode begins\n");
        //Interactive mode
        //Set the unused pointer to the NULL
        for (int i = 0; i < MAX_NUMBER_OF_THREADS; ++i) {
            t_ids[i] = 0;
            input_files[i] = NULL;
            is_exited[i] = 1;
        }
        //Create FE Thread and link it to the manage interactive mode
        if (pthread_create(&FE_thread_id, NULL, manageInteractiveMode, NULL) != 0) {
            fprintf(stderr, "Error creating FE thread \n");
            return 1;
        }
        //Wait for the interactive thread to finish
        pthread_join(FE_thread_id, NULL);
        return 0;
    }
}

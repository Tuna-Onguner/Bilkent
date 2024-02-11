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
 * Date: 19 November 2023, Sunday
 *
 * Description: (serverk) serverk.c file contains the implementation of the server program for the project.
 *                        The server program is responsible for handling the requests from the client program.
 */


// Libraries
#include "shareddefinitions.h"


// Constants
#define TRUE 1
#define FALSE 0
#define MAX_HASH_TABLE_SIZE 1024


// Hash table
struct KayValuePair {
    long int key;
    char *value;
};

struct HashEntry {
    long int key;
    long int offset;
    struct HashEntry *next;
};

struct HashTable {
    struct HashEntry *entries[MAX_HASH_TABLE_SIZE];
};

unsigned int hash(long int key) {
    unsigned int hash_val = 5381;

    while (key != 0) {
        hash_val = (hash_val << 5) + key % 10;
        key /= 10;
    }

    return hash_val % MAX_HASH_TABLE_SIZE;
}

void initHashTable(struct HashTable *ht) {
    for (int i = 0; i < MAX_HASH_TABLE_SIZE; i++) {
        ht->entries[i] = NULL;
    }
}

void insert(struct HashTable *ht, long int key, long int offset) {
    unsigned int index = hash(key);

    struct HashEntry *newEntry = (struct HashEntry *) malloc(sizeof(struct HashEntry));
    if (newEntry == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    newEntry->key = key;
    newEntry->offset = offset;
    newEntry->next = NULL;

    if (ht->entries[index] == NULL) {
        ht->entries[index] = newEntry;
    } else {
        newEntry->next = ht->entries[index];
        ht->entries[index] = newEntry;
    }
}

long int search(struct HashTable *ht, long int key) {
    unsigned int index = hash(key);

    struct HashEntry *current = ht->entries[index];
    while (current != NULL) {
        if (current->key == key) {
            return current->offset; // Key found, return the corresponding offset
        }
        current = current->next;
    }

    return -1; // Key not found
}


// Global variables
FILE **datastore;
Message *messages_buffer;

pthread_t FEThread, *workerThreads;

pthread_cond_t *worker_cond_vars;
pthread_mutex_t *worker_mutex_vars, *file_mutex_vars;
pthread_mutex_t busy_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t busy_cond = PTHREAD_COND_INITIALIZER;

mqd_t mq1;
mqd_t mq2;

char *fname, *mqname;
int dcount, tcount, vsize;
int t_index = 0, *is_busy, *next_offset;

struct HashTable ht;


// Worker threads function
void *funcWorkerThread(void *arg) {
    long thread_id = (long) arg;

    wait_on_cond_var:
    pthread_cond_wait(&worker_cond_vars[thread_id], &worker_mutex_vars[thread_id]);

    int valid_request = TRUE;

    while (TRUE) {

        Message request = messages_buffer[thread_id];
        int file_number = ((int) request.key) % dcount + 1;

        if (strcmp(request.request_type, "PUT") == 0) {
            printf("PUT request is received\n");
            long int hash_search = search(&ht, request.key);

            struct KayValuePair *kv = malloc(sizeof(struct KayValuePair));
            kv->key = request.key;
            kv->value = strdup(request.value); // Copy the value to avoid issues with dynamic memory

            //-lock is added by Ege
            pthread_mutex_lock(&file_mutex_vars[file_number - 1]);
            if (hash_search >= 0) {
                fseek(datastore[file_number - 1], hash_search, SEEK_SET);
            } else {
                fseek(datastore[file_number - 1], next_offset[file_number - 1], SEEK_SET);
                insert(&ht, request.key, next_offset[file_number - 1]);
                next_offset[file_number - 1] += vsize + 8;
            }


            fwrite(&(kv->key), sizeof(long int), 1, datastore[file_number - 1]);
            fwrite(kv->value, vsize, 1, datastore[file_number - 1]);

            pthread_mutex_unlock(&file_mutex_vars[file_number - 1]);

            free(kv->value);
            free(kv);

            request.operation_successful = 'S';

        } else if (strcmp(request.request_type, "GET") == 0) {
            printf("GET request is received\n");
            long int hash_search = search(&ht, request.key);

            if (hash_search >= 0) {
                struct KayValuePair *kv = malloc(sizeof(struct KayValuePair));

                fseek(datastore[file_number - 1], hash_search, SEEK_SET);
                fread(&(kv->key), sizeof(long int), 1, datastore[file_number - 1]);

                // Allocate memory for the value
                kv->value = malloc(vsize);

                // Read the value
                fread(kv->value, 1, vsize, datastore[file_number - 1]);

                request.operation_successful = 'S';
                request.value = strdup(request.value); // Copy the value to avoid issues with dynamic memory

                free(kv->value);
                free(kv);

            } else {
                request.operation_successful = 'U';
            }
        } else if (strcmp(request.request_type, "DEL") == 0) {
            printf("DEL request is received\n");
            long int hash_search = search(&ht, request.key);

            pthread_mutex_lock(&file_mutex_vars[file_number - 1]);

            if (hash_search >= 0) {
                struct HashEntry *current = ht.entries[hash(request.key)];
                struct HashEntry *previous = NULL;
                while (current != NULL) {
                    if (current->key == request.key) {
                        if (previous == NULL) {
                            ht.entries[hash(request.key)] = current->next;
                        } else {
                            previous->next = current->next;
                        }
                        free(current);
                        break;
                    }

                    previous = current;
                    current = current->next;
                }

                long int copy_offset = hash_search + vsize + 8;
                while (copy_offset < next_offset[file_number - 1]) {
                    struct KayValuePair *kv = malloc(sizeof(struct KayValuePair));

                    fseek(datastore[file_number - 1], copy_offset, SEEK_SET);
                    fread(&(kv->key), sizeof(long int), 1, datastore[file_number - 1]);
                    // Allocate memory for the value
                    kv->value = malloc(vsize);
                    // Read the value
                    fread(kv->value, 1, vsize, datastore[file_number - 1]);

                    fseek(datastore[file_number - 1], copy_offset - vsize - 8, SEEK_SET);
                    fwrite(&(kv->key), sizeof(long int), 1, datastore[file_number - 1]);
                    fwrite(kv->value, vsize, 1, datastore[file_number - 1]);

                    free(kv->value);
                    free(kv);
                    copy_offset += vsize + 8;
                }

                next_offset[file_number - 1] -= vsize + 8;
                ftruncate(fileno(datastore[file_number - 1]), next_offset[file_number - 1]);

                pthread_mutex_unlock(&file_mutex_vars[file_number - 1]);

                request.operation_successful = 'S';
            } else {
                request.operation_successful = 'U';
            }
        } else if (strcmp(request.request_type, "DMP") == 0) { // TODO: Check DMP
            printf("DMP request is received\n");
            FILE *fp;
            for (int i = 0; i < MAX_HASH_TABLE_SIZE; i++) {
                struct HashEntry *current = ht.entries[i];
                // Traverse the linked list at the current hash index
                while (current != NULL) {

                    struct KayValuePair *kv = malloc(sizeof(struct KayValuePair));
                    kv->key = current->key;
                    long int offset = current->offset;
                    int file_number_dmp = ((int) kv->key) % dcount + 1;
                    fseek(datastore[file_number_dmp - 1], offset, SEEK_SET);
                    fread(&(kv->key), sizeof(long int), 1, datastore[file_number_dmp - 1]);
                    // Allocate memory for the value
                    kv->value = malloc(vsize);
                    // Read the value
                    fread(kv->value, 1, vsize, datastore[file_number_dmp - 1]);

                    // Oen a .txt file whose name is request.value and write the key and value pairs in it
                    fp = fopen(request.value, "w");
                    fprintf(fp, "%ld %s\n", kv->key, kv->value);

                    // Free the allocated memory
                    free(kv->value);
                    free(kv);

                    // Advance the pointer
                    current = current->next;
                }
            }

            fclose(fp);
            request.operation_successful = 'S';
        } else {
            fprintf(stderr, "Invalid request type: %s\n", request.request_type);
            valid_request = FALSE;
            exit(EXIT_FAILURE);
        }

        if (valid_request) {
            char message_binary[MAX_MESSAGE_SIZE];
            encodeMessage(&request, message_binary, vsize);

            if (mq_send(mq2, message_binary, MAX_MESSAGE_SIZE, 0) == -1) {
                perror("mq_send failed");
                exit(EXIT_FAILURE);
            }
            //printf("Request sent\n");
        }

        is_busy[thread_id] = FALSE;
        pthread_cond_signal(&busy_cond);
        goto wait_on_cond_var;
    }

    return NULL;
}


// Front-end thread function
void *funcFEThread() {
    for (long i = 0; i < tcount; i++) {
        if (pthread_create(&workerThreads[i], NULL, funcWorkerThread, (void *) i) != 0) {
            fprintf(stderr, "Error creating worker thread #%ld\n", i);
            exit(EXIT_FAILURE);
        }
    }

    is_busy = malloc(tcount * sizeof(int));
    for (int i = 0; i < tcount; i++) {
        is_busy[i] = FALSE;
    }

    struct mq_attr mq1_attrs;

    while (TRUE) {
        mq_getattr(mq1, &mq1_attrs);

        if (mq1_attrs.mq_curmsgs > 0) {

            char message_binary[MAX_MESSAGE_SIZE];
            if (mq_receive(mq1, message_binary, mq1_attrs.mq_msgsize, NULL) == -1) {
                perror("mq_receive failed");
                exit(EXIT_FAILURE);
            }
            Message *request = malloc(sizeof(Message));
            decodeMessage(message_binary, request, vsize);
            //printf("Request: %s, Key: %ld, Value: %s, Thread ID: %lu is before completion: %c\n",
            //       request->request_type, request->key, request->value, request->t_id, request->operation_successful);
            if (strcmp(request->request_type, "QTS") == 0) {
                for (int i = 0; i < tcount; i++) {
                    pthread_cancel(workerThreads[i]);
                }

                free(request);
                free(is_busy);

                return NULL;
            }
            int busy_threads = 0;

            look_for_non_busy_thread:
            pthread_cond_wait(&busy_cond, &busy_mutex);
            for (int i = 0; i < tcount; i++) {
                if (is_busy[i]) {
                    busy_threads++;
                    continue;
                }
                t_index = i;
                busy_threads = 0;
                break;
            }

            if (busy_threads == tcount) {
                goto look_for_non_busy_thread;
            }
            //printf("Request submitted\n");
            messages_buffer[t_index] = *request;

            pthread_cond_signal(&worker_cond_vars[t_index]);
            is_busy[t_index] = TRUE;
        }
    }
}

// Main function
int main(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "d:f:t:s:m:")) != -1) {
        switch (opt) {
            case 'd': {
                dcount = atoi(optarg);
                if (dcount < 1 || dcount > 5) {
                    fprintf(stderr, "Invalid value for `dcount`. `dcount` must be between 1 and 5\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'f': {
                fname = optarg;
                break;
            }
            case 't': {
                tcount = atoi(optarg);
                if (tcount < 1 || tcount > 5) {
                    fprintf(stderr, "Invalid value for `tcount`. `tcount` must be between 1 and 5\n");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 's': {
                vsize = atoi(optarg);
                if (vsize < 32 || vsize > 1024 || vsize % 32 != 0) {
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
                fprintf(stderr, "Usage: %s -d <dcount> -f <fname> -t <tcount> -s <vsize> -m <mqname>\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    datastore = malloc(dcount * sizeof(FILE *));
    messages_buffer = malloc(tcount * sizeof(Message));

    workerThreads = malloc(tcount * sizeof(pthread_t));

    char dataFileNames[dcount][strlen(fname) + 6];
    for (int i = 0; i < dcount; i++) {
        sprintf(dataFileNames[i], "%s%d.bin", fname, i + 1);
        datastore[i] = fopen(dataFileNames[i], "wb+");
    }

    worker_cond_vars = malloc(tcount * sizeof(pthread_cond_t));
    for (int i = 0; i < tcount; i++) {
        pthread_cond_init(&worker_cond_vars[i], NULL);
    }

    worker_mutex_vars = malloc(tcount * sizeof(pthread_mutex_t));
    for (int i = 0; i < tcount; i++) {
        pthread_mutex_init(&worker_mutex_vars[i], NULL);
    }

    file_mutex_vars = malloc(dcount * sizeof(pthread_mutex_t));
    for (int i = 0; i < dcount; i++) {
        pthread_mutex_init(&file_mutex_vars[i], NULL);
    }

    next_offset = malloc(dcount * sizeof(int));
    for (int i = 0; i < dcount; i++) {
        next_offset[i] = 0;
    }

    initHashTable(&ht);

    char mqname1[strlen(mqname) + 3];
    char mqname2[strlen(mqname) + 3];
    sprintf(mqname1, "/%s1", mqname);
    sprintf(mqname2, "/%s2", mqname);

    mq1 = mq_open(mqname1, O_CREAT | O_RDWR | O_CLOEXEC, 0666, NULL);
    mq2 = mq_open(mqname2, O_CREAT | O_RDWR | O_CLOEXEC, 0666, NULL);

    if (mq1 == -1 || mq2 == -1) {
        perror("mq_open failed");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&FEThread, NULL, funcFEThread, NULL) != 0) {
        fprintf(stderr, "Error creating the FE thread\n");
        exit(EXIT_FAILURE);
    }

    // Wait for the FE thread to exit
    if (pthread_join(FEThread, NULL) != 0) {
        fprintf(stderr, "Error joining the FE thread\n");
        exit(EXIT_FAILURE);
    }

    // Close the data files
    for (int i = 0; i < dcount; i++) {
        fclose(datastore[i]);
    }

    // Free the allocated memory
    free(datastore);
    free(messages_buffer);

    free(workerThreads);

    free(worker_cond_vars);
    free(worker_mutex_vars);
    free(file_mutex_vars);

    free(next_offset);

    //close message queues
    mq_close(mq1);
    mq_close(mq2);

    //unlink message queues
    mq_unlink(mqname1);
    mq_unlink(mqname2);

    return 0;
}

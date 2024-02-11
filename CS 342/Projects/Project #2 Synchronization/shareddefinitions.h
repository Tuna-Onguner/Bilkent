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
 * Description: shareddefinitions.h file contains the shared definitions
 */

#ifndef SYNCHRONIZATION_SHAREDDEFINITIONS_H
#define SYNCHRONIZATION_SHAREDDEFINITIONS_H

#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_MESSAGE_SIZE 1048
/*
typedef struct {
    pthread_cond_t server_FE_cond_var;
    pthread_mutex_t server_FE_mutex_var;
    pthread_cond_t client_FE_cond_var;
    pthread_mutex_t client_FE_mutex_var;
    pthread_cond_t client_exiting_cond_var;
    pthread_mutex_t client_exiting_mutex_var;
} SharedData;
*/

typedef struct {
    char request_type[4];              // PUT, GET, DEL
    long key;                          // Assuming values are stored as 8-byte integers
    char *value;                       // Assuming data is stored as char* with client_vsize length
    char operation_successful;         // S, U
    pthread_t t_id;                    // Thread ID
} Message;

Message *createMessage(const char *request_type, long key, const char *value, char operation_successful,
                       int vsize) {
    Message *msg = malloc(sizeof(Message));

    // Ensure null-termination for request_type
    strncpy(msg->request_type, request_type, sizeof(msg->request_type) - 1);
    msg->request_type[sizeof(msg->request_type) - 1] = '\0';

    msg->key = key;
    msg->value = NULL;  // Initialize to NULL to avoid issues with free
    if (value != NULL) {
        msg->value = malloc(vsize);
        strncpy(msg->value, value, vsize - 1);
        msg->value[vsize - 1] = '\0';  // Ensure null-termination
    }
    msg->t_id = pthread_self();
    msg->operation_successful = operation_successful;

    return msg;
}

void encodeMessage(const Message *msg, char *buffer, int client_vsize) {
    size_t offset = 0;

    // Copy request_type along with null-terminator
    memcpy(buffer + offset, msg->request_type, sizeof(msg->request_type));
    offset += sizeof(msg->request_type);

    // Copy key
    memcpy(buffer + offset, &msg->key, sizeof(msg->key));
    offset += sizeof(msg->key);

    // Copy value
    if (msg->value != NULL) {
        memcpy(buffer + offset, msg->value, client_vsize);
    }
    offset += client_vsize;

    // Copy operation_successful
    memcpy(buffer + offset, &msg->operation_successful, sizeof(char));
    offset += sizeof(char);

    // Copy t_id
    memcpy(buffer + offset, &msg->t_id, sizeof(pthread_t));
}

void decodeMessage(const char *buffer, Message *msg, int client_vsize) {
    size_t offset = 0;

    // Copy request_type
    memcpy(msg->request_type, buffer + offset, sizeof(msg->request_type));
    offset += sizeof(msg->request_type);

    // Copy key
    memcpy(&msg->key, buffer + offset, sizeof(msg->key));
    offset += sizeof(msg->key);

    // Allocate memory for value and copy
    if (buffer+offset != NULL) {
        msg->value = malloc(client_vsize);
        memcpy(msg->value, buffer + offset, client_vsize);
    }
    offset += client_vsize;

    // Copy operation_successful
    memcpy(&msg->operation_successful, buffer + offset, sizeof(char));
    offset += sizeof(char);

    // Copy t_id
    memcpy(&msg->t_id, buffer + offset, sizeof(pthread_t));
}

#endif //SYNCHRONIZATION_SHAREDDEFINITIONS_H

/*
 * Bilkent University
 * Semester of Fall 2023
 *
 * Department of Computer Engineering
 *
 * CS342 Operating Systems
 * Project #4: A Very Simple File System (VSFS)
 *
 * Section 1
 * Instructor: İbrahim Körpeoğlu
 *
 * Authors: Deniz Tuna Onguner (22001788) and
 *          Sarper Arda Bakır (21902781)
 *
 * version 1.0
 * Date: 20 December 2023, Wednesday
 *
 * Description: Implementation of the VSFS.
 */


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vsfs.h"


// Constants =====================================================================
#define FAT_ENTRY_SIZE          4           // bytes
#define DIR_ENTRY_SIZE          128         // bytes

#define FAT_ENTRY_COUNT         16384       // entries
#define DIR_ENTRY_COUNT         128         // entries

#define FAT_ENTRY_PER_BLOCK     512         // entries
#define DIR_ENTRY_PER_BLOCK     16          // entries

#define FILE_DESC_CONS          101         // magic number for file descriptors

#define MAX_FILE_NAMES          30          // characters
#define MAX_OPEN_FILES          16          // files
#define MAX_FILE_COUNT          128         // files

#define MAX_DISK_SIZE           1<<23       // bytes
#define MIN_DISK_SIZE           1<<18       // bytes
// ===============================================================================


// Globals Variables =============================================================
int vs_fd;              // File descriptor of the Linux file.
int open_files_count;   // Number of files currently open (max 16).
// ===============================================================================


// Structures ====================================================================
struct {                // No special attributes are needed for the super block.
    int self;           // Position of the super block in the Linux file.
    int fat_block;      // Position of the FAT in the Linux file.
    int dir_block;      // Position of the root directory in the Linux file.
    int data_block;     // Position of the data blocks in the Linux file.
    int file_count;     // Number of files in the root directory.
} super_block; // The super block as a global variable

struct {            // Each entry in the FAT is 4 bytes.
    int used: 1;    // 0 => Block is free; 1 => Block is used.
    //int eof: 1;   // !!! DEPRECATED !!!
    int next: 31;   // Block number of the next block in the file.
} fat[FAT_ENTRY_COUNT]; // The FAT as a global variable

struct __attribute__((packed))  /* __attribute__((packed)): 'not to pad the structure' */  dir_entry {
    char name[MAX_FILE_NAMES];  // File name.
    int size;                   // File size in bytes.
    int first_block;            // Block number of the first data block.

    // each dir_entry must be exactly 128 bytes
    char padding[128 - sizeof(char) * MAX_FILE_NAMES - sizeof(int) * 2];
    // padding is used to make each dir_entry exactly 128 bytes
} root_dir[DIR_ENTRY_COUNT]; // The root directory as a global variable

struct {
    int file_index; // Index of the file in the root_dir array.
    int mode;       // MODE_READ or MODE_APPEND -- so, 0 or 1.
    //int offset;   // !!! DEPRECATED !!!
} open_files[MAX_OPEN_FILES]; // The open files array as a global variable
// ===============================================================================


// Helper functions ==============================================================
int read_block(void *block, int k) {
    off_t offset = k * BLOCKSIZE;
    lseek(vs_fd, offset, SEEK_SET);


    if (read(vs_fd, block, BLOCKSIZE) != BLOCKSIZE) {
        printf("read error\n");
        return -1;
    }

    return 0;
}

int write_block(void *block, int k) {
    off_t offset = k * BLOCKSIZE;
    lseek(vs_fd, offset, SEEK_SET);

    if (write(vs_fd, block, BLOCKSIZE) != BLOCKSIZE) {
        printf("write error\n");
        return -1;
    }

    return 0;
}

int min(int a, int b) { // Returns the minimum of two integers. Used in vsappend().
    return a < b ? a : b;
}
// ===============================================================================


int vsformat(char *vdiskname, int m) {
    const int disk_size = 1 << m; // 2^m bytes
    // Step 1: Check if the disk size is valid
    if (disk_size < MIN_DISK_SIZE || disk_size > MAX_DISK_SIZE) {
        printf("Invalid disk size\n\tMinimum disk size: %d bytes\n\tMaximum disk size: %d bytes\n",
               MIN_DISK_SIZE, MAX_DISK_SIZE);
        return -1; // Invalid disk size
    }

    // Step 2: Create the Linux file
    char command[1000];
    const int block_count = (int) (disk_size / BLOCKSIZE);

    sprintf(command, "dd if=/dev/zero of=%s bs=%d count=%d",
            vdiskname, BLOCKSIZE, block_count);

    system(command);

    // Step 3: Open the Linux file
    vs_fd = open(vdiskname, O_RDWR);
    if (vs_fd == -1) {
        printf("Error opening the Linux file\n");
        return -1; // Error opening the Linux file
    }

    // Step 4: Initialize the super block
    super_block.self = 0;
    super_block.fat_block = 1;
    super_block.dir_block = 33;
    super_block.data_block = 41;
    super_block.file_count = 0;

    // Step 5: Initialize the FAT
    for (int i = 0; i < FAT_ENTRY_COUNT; i++) {
        fat[i].used = false;
        //fat[i].eof = false; // !!! DEPRECATED !!!
        fat[i].next = -1;
    }

    // Step 6: Initialize the root directory
    for (int i = 0; i < DIR_ENTRY_COUNT; i++) {
        root_dir[i].name[0] = '\0';
        root_dir[i].size = 0;
        root_dir[i].first_block = -1;
    }

    // Step 7: Initialize the open files array
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        open_files[i].file_index = -1;
        open_files[i].mode = -1;
        //open_files[i].offset = -1; // !!! DEPRECATED !!!
    }

    open_files_count = 0;

    // Step 8: Write the super block to the disk
    void *block = malloc(BLOCKSIZE);
    memcpy(block, &super_block, sizeof(super_block));
    write_block(block, super_block.self);

    // Step 9: Write the FAT to the disk
    for (int i = 0; i < FAT_ENTRY_COUNT; i += FAT_ENTRY_PER_BLOCK) {
        memcpy(block, fat + i, FAT_ENTRY_PER_BLOCK * FAT_ENTRY_SIZE);
        write_block(block, super_block.fat_block + i / FAT_ENTRY_PER_BLOCK);
    }

    // Step 10: Write the root directory to the disk
    for (int i = 0; i < DIR_ENTRY_COUNT; i += DIR_ENTRY_PER_BLOCK) {
        memcpy(block, root_dir + i, DIR_ENTRY_PER_BLOCK * DIR_ENTRY_SIZE);
        write_block(block, super_block.dir_block + i / DIR_ENTRY_PER_BLOCK);
    }

    // Step 11: Close the Linux file
    fsync(vs_fd);
    close(vs_fd);

    // Step 12: Free the block
    free(block);

    return 0;
}

int vsmount(char *vdiskname) {
    // Step 1: Open the Linux file
    vs_fd = open(vdiskname, O_RDWR);
    if (vs_fd == -1) {
        printf("Error opening the Linux file\n");
        return -1; // Error opening the Linux file
    }

    // Step 2: Read the super block from the disk
    void *block = malloc(BLOCKSIZE);
    read_block(block, 0);
    memcpy(&super_block, block, sizeof(super_block));

    // Step 3: Read the FAT from the disk
    for (int i = 0; i < FAT_ENTRY_COUNT; i += FAT_ENTRY_PER_BLOCK) {
        read_block(block, super_block.fat_block + i / FAT_ENTRY_PER_BLOCK);
        memcpy(fat + i, block, FAT_ENTRY_PER_BLOCK * FAT_ENTRY_SIZE);
    }

    // Step 4: Read the root directory from the disk
    for (int i = 0; i < DIR_ENTRY_COUNT; i += DIR_ENTRY_PER_BLOCK) {
        read_block(block, super_block.dir_block + i / DIR_ENTRY_PER_BLOCK);
        memcpy(root_dir + i, block, DIR_ENTRY_PER_BLOCK * DIR_ENTRY_SIZE);
    }

    // Step 5: Initialize the open files array
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        open_files[i].file_index = -1;
        open_files[i].mode = -1;
        //open_files[i].offset = -1; // !!! DEPRECATED !!!
    }

    // Step 6: Initialize the open_files_count
    open_files_count = 0;

    // Step 7: Free the block
    free(block);

    return 0;
}

int vsumount() {
    // Step 1: Write the super block to the disk
    void *block = malloc(BLOCKSIZE);
    memcpy(block, &super_block, sizeof(super_block));
    write_block(block, super_block.self);

    // Step 2: Write the FAT to the disk
    for (int i = 0; i < FAT_ENTRY_COUNT; i += FAT_ENTRY_PER_BLOCK) {
        memcpy(block, fat + i, FAT_ENTRY_PER_BLOCK * FAT_ENTRY_SIZE);
        write_block(block, super_block.fat_block + i / FAT_ENTRY_PER_BLOCK);
    }

    // Step 3: Write the root directory to the disk
    for (int i = 0; i < DIR_ENTRY_COUNT; i += DIR_ENTRY_PER_BLOCK) {
        memcpy(block, root_dir + i, DIR_ENTRY_PER_BLOCK * DIR_ENTRY_SIZE);
        write_block(block, super_block.dir_block + i / DIR_ENTRY_PER_BLOCK);
    }

    // Step 4: Close the Linux file
    fsync(vs_fd);
    close(vs_fd);

    // Step 5: Free the block
    free(block);

    return 0;
}

int vscreate(char *filename) {
    // Step 1: Check if the file already exists
    for (int i = 0; i < DIR_ENTRY_COUNT; i++) {
        if (strcmp(root_dir[i].name, filename) == 0) {
            printf("File already exists\n");
            return -1; // File already exists
        }
    }

    // Step 2: Check if the root directory is full
    if (super_block.file_count >= MAX_FILE_COUNT) {
        printf("Root directory is full\n");
        return -1; // Root directory is full
    }

    // Step 3: Find a free entry in the root directory
    int file_index = -1;
    for (int i = 0; i < DIR_ENTRY_COUNT; i++) {
        if (root_dir[i].name[0] == '\0') {
            file_index = i;
            break;
        }
    }

    // Step 4: Set the name, size, and first_block attributes of the file
    strcpy(root_dir[file_index].name, filename);
    root_dir[file_index].size = 0;
    root_dir[file_index].first_block = -1;

    // Step 5: Increment the file_count in the super_block
    super_block.file_count += 1;

    return 0;
}

int vsopen(char *filename, int mode) {
    // Step 1: Check if the file exists
    int file_index = -1;
    for (int i = 0; i < DIR_ENTRY_COUNT; i++) {
        if (strcmp(root_dir[i].name, filename) == 0) {
            file_index = i;
            break;
        }
    }

    if (file_index == -1) {
        printf("File does not exist\n");
        return -1; // File does not exist
    }

    // Step 2: Check if the file is already open
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_files[i].file_index == file_index) {
            printf("File is already open\n");
            return -1; // File is already open
        }
    }

    // Step 3: Find a free entry in the open_files array
    int fd = -1;
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_files[i].file_index == -1) {
            fd = i;
            break;
        }
    }

    if (fd == -1) {
        printf("Too many open files\n");
        return -1; // Too many open files
    }

    // Step 4: Set the file_index, mode, and offset attributes of the file in the open_files array
    open_files[fd].file_index = file_index;
    open_files[fd].mode = mode;
    //open_files[fd].offset = 0; // !!! DEPRECATED !!!

    // Step 5: Increment the open_files_count
    open_files_count++;

    return fd + FILE_DESC_CONS; // Return the file descriptor using the magic number
}

int vssize(int fd) {
    // Step 1: Check if the file descriptor is valid
    if (fd < FILE_DESC_CONS || fd >= FILE_DESC_CONS + MAX_OPEN_FILES) {
        printf("Invalid file descriptor\n");
        return -1; // Invalid file descriptor
    }

    // Step 2: Get the file index from the open_files array
    int file_index = open_files[fd - FILE_DESC_CONS].file_index;

    // Step 3: Access the file in the root_dir array
    struct dir_entry *file = &root_dir[file_index];

    // Step 4: Return the size of the file
    return file->size;
}

int vsclose(int fd) {
    // Step 1: Check if the file descriptor is valid
    if (fd < FILE_DESC_CONS || fd >= FILE_DESC_CONS + MAX_OPEN_FILES) {
        printf("Invalid file descriptor\n");
        return -1; // Invalid file descriptor
    }

    // Step 2: Mark the corresponding entry in the open_files array as free
    open_files[fd - FILE_DESC_CONS].file_index = -1;

    // Step 3: Decrement the open_files_count
    open_files_count--;

    return 0;
}

int vsread(int fd, void *buf, int n) {
    // Step 1: Check if the file descriptor is valid
    if (fd < FILE_DESC_CONS || fd >= FILE_DESC_CONS + MAX_OPEN_FILES) {
        printf("Invalid file descriptor\n");
        return -1; // Invalid file descriptor
    }

    // Step 2: Get the file index from the open_files array
    int file_index = open_files[fd - FILE_DESC_CONS].file_index;

    // Step 3: Access the file in the root_dir array
    struct dir_entry *file = &root_dir[file_index];

    // Step 4: Check if the file is open in MODE_READ
    if (open_files[fd - FILE_DESC_CONS].mode != MODE_READ) {
        printf("File is not open in MODE_READ\n");
        return -1; // File is not open in MODE_READ
    }

    // Step 5: Calculate the number of bytes to read
    int bytes_to_read = min(n, file->size);

    // Step 6: Read the data from the file
    void *block = malloc(BLOCKSIZE);
    int current_block = file->first_block, bytes_read = 0;
    while (bytes_read < bytes_to_read) {
        read_block(block, current_block + super_block.data_block);
        int bytes_to_copy = min(bytes_to_read - bytes_read, BLOCKSIZE);
        memcpy(buf + bytes_read, block, bytes_to_copy); // Copy the data to the buffer
        bytes_read += bytes_to_copy; // Update the number of bytes read
        current_block = fat[current_block].next; // Go to the next block via the FAT
    }

    free(block); // Free the block

    // Step 7: Return the number of bytes read
    return bytes_read;
}

int vsappend(int fd, void *buf, int n) {
    // Step 1: Check if the file descriptor is valid
    if (fd < FILE_DESC_CONS || fd >= FILE_DESC_CONS + MAX_OPEN_FILES) {
        printf("Invalid file descriptor\n");
        return -1; // Invalid file descriptor
    }

    // Step 2: Get the file index from the open_files array
    int file_index = open_files[fd - FILE_DESC_CONS].file_index;

    // Step 3: Access the file in the root_dir array
    struct dir_entry *file = &root_dir[file_index];

    // Step 4: Check if the file is open in MODE_APPEND
    if (open_files[fd - FILE_DESC_CONS].mode != MODE_APPEND) {
        printf("File is not open in MODE_READ\n");
        return -1; // File is not open in MODE_APPEND
    }

    // Step 5: Write the data to the file
    void *block = malloc(BLOCKSIZE);
    int current_block = file->first_block;
    int bytes_written = 0;
    int block_offset = file->size % BLOCKSIZE; // Add a new variable to keep track of the offset within the block
    while (bytes_written < n) {
        if (current_block == -1 || block_offset == 0) { // Check if the block is full
            int new_block = -1;
            // Find a free block
            for (int i = 0; i < FAT_ENTRY_COUNT; i++) {
                if (!fat[i].used) {
                    new_block = i;
                    break;
                }
            }
            // Check if there is a free block
            if (new_block == -1) {
                free(block);
                printf("No free blocks\n");
                return -1; // No free blocks
            }

            // Initialize the new block
            fat[new_block].used = true;
            //fat[new_block].eof = true; // !!! DEPRECATED !!!

            // Update the FAT
            if (current_block != -1) {
                //fat[current_block].eof = false; // !!! DEPRECATED !!!
                fat[current_block].next = new_block;
            } else {
                file->first_block = new_block;
            }

            // Update the current block
            current_block = new_block;
            block_offset = 0; // Reset the block offset
        }

        read_block(block, current_block + super_block.data_block);
        int bytes_to_copy = min(n - bytes_written, BLOCKSIZE - block_offset); // Adjust the bytes to copy
        memcpy(block + block_offset, buf + bytes_written, bytes_to_copy); // Adjust the destination pointer
        write_block(block, current_block + super_block.data_block);
        bytes_written += bytes_to_copy;
        block_offset += bytes_to_copy; // Update the block offset
    }

    free(block); // Free the block

    // Step 6: Update the current offset in the open_files array
    //open_files[fd - FILE_DESC_CONS].offset += bytes_written; // !!! DEPRECATED !!!

    // Step 7: Update the file size in the root_dir array
    file->size += bytes_written;

    // Step 8: Return the number of bytes written
    return bytes_written;
}

int vsdelete(char *filename) {
    // Step 1: Check if the file exists
    int file_index = -1;
    for (int i = 0; i < DIR_ENTRY_COUNT; i++) {
        if (strcmp(root_dir[i].name, filename) == 0) {
            file_index = i;
            break;
        }
    }

    if (file_index == -1) {
        printf("File does not exist\n");
        return -1; // File does not exist
    }

    // Step 2: Check if the file is open
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_files[i].file_index == file_index) {
            printf("File is open\n");
            return -1; // File is open
        }
    }

    // Step 3: Delete all the blocks of the file in the FAT
    int current_block = root_dir[file_index].first_block, next_block;
    while (current_block != -1) {
        next_block = fat[current_block].next;
        fat[current_block].used = false;
        //fat[current_block].eof = false; // !!! DEPRECATED !!!
        fat[current_block].next = -1;
        current_block = next_block;
    }

    // Step 4: Set the name, size, and first_block attributes of the file to their initial values
    root_dir[file_index].name[0] = '\0';
    root_dir[file_index].size = 0;
    root_dir[file_index].first_block = -1;

    // Step 5: Decrement the file_count in the super_block
    super_block.file_count -= 1;

    return 0;
}

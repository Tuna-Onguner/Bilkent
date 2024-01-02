/*
 * Bilkent University
 * Semester of Fall 2023
 *
 * Department of Computer Engineering
 *
 * CS342 Operating Systems
 * Project #3: Virtual Memory and Paging
 *
 * Section 1
 * Instructor: İbrahim Körpeoğlu
 *
 * Authors: Deniz Tuna Onguner (22001788) and Alper Göçmen (22002948)
 *
 * version 1.0
 * Date: 4 December 2023, Monday
 *
 * Description: Implementation of the program `memsim`.
 * A simulator, that will simulate the memory accesses and the paging behaviour of a simulated program.
 */

#include <ctype.h>
#include <unistd.h>
#include <string.h>
// #include <bits/getopt_core.h> // might be needed for some compilers or operating systems
#include "pagetable.c"

#define TRUE                1
#define FALSE               0

#define VIRTUAL_MEM_SIZE    65536       // Virtual Memory Size in Bytes
#define PAGE_COUNT          1024        // Number of Pages
#define PAGE_SIZE           64          // Page Size in Bytes

unsigned short tick = 0, level = 0, fcount = 0;    // tick, level, frame count
unsigned short tick_counter, next_frame_ptr;       // tick counter, next frame pointer

unsigned short PHYSICAL_MEM_SIZE;       // Physical Memory Size in Bytes
unsigned char *PHYSICAL_MEM;            // Physical Memory

// Page Replacement Algorithms, namely FIFO, LRU, CLOCK, and ECLOCK
typedef enum PageReplacementAlgorithm {
    FIFO,       // First In First Out
    LRU,        // Least Recently Used
    CLOCK,      // Second Chance
    ECLOCK      // Enhanced Second Chance
} PRA;

// helper functions
/**
 * Capitalizes all characters in a string.
 * @param str The string to be capitalized.
 */
void capitalize(char *str) {
    while (*str) {
        *str = (char) toupper((unsigned char) *str);
        str = str + 1;
    }
}

/**
 * Gets the page replacement algorithm from a string.
 * @param pra_str The string to be converted to a page replacement algorithm.
 * @return The page replacement algorithm.
 */
PRA getPRAlgorithm(const char *pra_str) {
    switch (*pra_str) {
        case 'F':
            if (strcmp(pra_str, "FIFO") == 0) {
                return FIFO;
            }
            break;
        case 'L':
            if (strcmp(pra_str, "LRU") == 0) {
                return LRU;
            }
            break;
        case 'C':
            if (strcmp(pra_str, "CLOCK") == 0) {
                return CLOCK;
            }
            break;
        case 'E':
            if (strcmp(pra_str, "ECLOCK") == 0) {
                return ECLOCK;
            }
            break;
        default:
            break;
    }

    fprintf(stderr, "Invalid algorithm: %s\n", pra_str);
    exit(EXIT_FAILURE);
}

// page replacement algorithms
void fifo(struct PageTable *pt, unsigned const short VPN, FILE *swapfile, unsigned short *fn) {
    unsigned short vpn;
    // Get the page table entry of the frame pointed by next_frame_ptr
    PTE *pte = frameNumberToPageTableEntry(pt, next_frame_ptr, &vpn);

    if (pte != NULL) { // NULL means that the frame is empty; otherwise, the frame is full
        if (pte->m == 1) { // The page in the frame is modified, should be written to the swap file
            fseek(swapfile, vpn * PAGE_SIZE, SEEK_SET); // Seek to the beginning of the page in the swap file
            fwrite(PHYSICAL_MEM + next_frame_ptr * PAGE_SIZE, // Write the page in the frame to the swap file
                   sizeof(unsigned char), PAGE_SIZE, swapfile);

            pte->m = 0; // Set the mBit to 0; the modified page is written to the swap file
        }

        pte->v = 0; // Set the vBit to 0; page is not in physical memory anymore
    }

    fseek(swapfile, VPN * PAGE_SIZE, SEEK_SET);
    fread(PHYSICAL_MEM + next_frame_ptr * PAGE_SIZE,
          sizeof(unsigned char), PAGE_SIZE, swapfile);

    *fn = next_frame_ptr; // Get the frame number that the page is loaded into
    next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
}

void lru(struct PageTable *pt, unsigned const short VPN, FILE *swapfile, unsigned short *fn) {
    unsigned short vpn, min_age = 63;
    // Find the page with the minimum age
    for (int i = (int) next_frame_ptr; i < fcount; i++) {
        // Get the page table entry of the frame pointed by next_frame_ptr
        PTE *pte = frameNumberToPageTableEntry(pt, i, &vpn);

        if (pte != NULL && pte->age <= min_age) { // The page is referenced
            min_age = pte->age; // Set the min_age to the age of the page
            next_frame_ptr = i; // Set the min_age_frame to the frame number of the page
        }
    }

    PTE *pte = frameNumberToPageTableEntry(pt, next_frame_ptr, &vpn);

    if (pte != NULL) {
        if (pte->m == 1) { // The page in the frame is modified
            fseek(swapfile, vpn * PAGE_SIZE, SEEK_SET); // Seek to the beginning of the page in the swap file
            fwrite(PHYSICAL_MEM + next_frame_ptr * PAGE_SIZE, // Write the page in the frame to the swap file
                   sizeof(unsigned char), PAGE_SIZE, swapfile);

            pte->m = 0; // Set the mBit to 0; the modified page is written to the swap file
        }

        pte->v = 0; // Set the vBit to 0; page is not in physical memory anymore
    }

    fseek(swapfile, VPN * PAGE_SIZE, SEEK_SET); // Seek to the beginning of the page in the swap file
    fread(PHYSICAL_MEM + next_frame_ptr * PAGE_SIZE, // Read the page from the swap file
          sizeof(unsigned char), PAGE_SIZE, swapfile);

    *fn = next_frame_ptr; // Get the frame number that the page is loaded into
    next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
}

void clock(struct PageTable *pt, unsigned const short VPN, FILE *swapfile, unsigned short *fn) {
    PTE *pte;
    unsigned short vpn;

    while (TRUE) { // Loop until the available frame is found
        // Get the page table entry of the frame pointed by next_frame_ptr
        pte = frameNumberToPageTableEntry(pt, next_frame_ptr, &vpn);
        if (pte != NULL && pte->r == 1) { // The page is referenced
            pte->r = 0; // Set the rBit to 0
            next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
        } else if (pte == NULL || pte->r == 0) { // The available frame is found
            break; // Break the loop
        }
    }

    if (pte != NULL) {
        if (pte->m == 1) { // The page in the frame is modified
            fseek(swapfile, vpn * PAGE_SIZE, SEEK_SET); // Seek to the beginning of the page in the swap file
            fwrite(PHYSICAL_MEM + next_frame_ptr * PAGE_SIZE, // Write the page in the frame to the swap file
                   sizeof(unsigned char), PAGE_SIZE, swapfile);

            pte->m = 0; // Set the mBit to 0; the modified page is written to the swap file
        }

        pte->v = 0; // Set the vBit to 0; page is not in physical memory anymore
    }

    fseek(swapfile, VPN * PAGE_SIZE, SEEK_SET); // Seek to the beginning of the page in the swap file
    fread(PHYSICAL_MEM + next_frame_ptr * PAGE_SIZE, // Read the page from the swap file
          sizeof(unsigned char), PAGE_SIZE, swapfile);

    *fn = next_frame_ptr; // Get the frame number that the page is loaded into
    next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
}

void eclock(struct PageTable *pt, unsigned const short VPN, FILE *swapfile, unsigned short *fn) {
    PTE *pte;
    // Initialize the victim_found to FALSE
    unsigned short vpn, victim_found = FALSE;

    for (int i = 0; i < fcount; i++) {
        // Get the page table entry of the frame pointed by next_frame_ptr
        pte = frameNumberToPageTableEntry(pt, next_frame_ptr, &vpn);

        if (pte == NULL || (pte->r == 0 && pte->m == 0)) { // The available frame is found
            victim_found = TRUE;
            break;
        }

        next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
    }

    if (!victim_found) {
        for (int i = 0; i < fcount; i++) {
            pte = frameNumberToPageTableEntry(pt, next_frame_ptr, &vpn);

            if (pte == NULL || (pte->r == 0 && pte->m == 1)) { // The available frame is found
                victim_found = TRUE;
                break;
            }

            pte->r = 0; // Set the rBit to 0
            next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
        }
    }

    if (!victim_found) {
        for (int i = 0; i < fcount; i++) {
            pte = frameNumberToPageTableEntry(pt, next_frame_ptr, &vpn);

            if (pte == NULL || (pte->r == 0 && pte->m == 0)) { // The available frame is found
                victim_found = TRUE;
                break;
            }

            pte->r = 0; // Set the rBit to 0
            next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
        }
    }

    if (!victim_found) {
        for (int i = 0; i < fcount; i++) {
            pte = frameNumberToPageTableEntry(pt, next_frame_ptr, &vpn);

            if (pte == NULL || (pte->r == 0 && pte->m == 1)) { // The available frame is found
                victim_found = TRUE;
                break;
            }

            pte->r = 0; // Set the rBit to 0
            next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
        }
    }

    if (pte != NULL) {
        if (pte->m == 1) { // The page in the frame is modified
            fseek(swapfile, vpn * PAGE_SIZE, SEEK_SET); // Seek to the beginning of the page in the swap file
            fwrite(PHYSICAL_MEM + pte->frame * PAGE_SIZE, // Write the page in the frame to the swap file
                   sizeof(unsigned char), PAGE_SIZE, swapfile);

            // Reset the removed page table entry
            pte->m = 0;
        }

        pte->r = 0;
        pte->v = 0; // Set the vBit to 0; page is not in physical memory anymore
    }

    fseek(swapfile, VPN * PAGE_SIZE, SEEK_SET); // Seek to the beginning of the page in the swap file
    fread(PHYSICAL_MEM + next_frame_ptr * PAGE_SIZE, // Read the page from the swap file
          sizeof(unsigned char), PAGE_SIZE, swapfile);

    *fn = next_frame_ptr; // Get the frame number that the page is loaded into
    next_frame_ptr = (next_frame_ptr + 1) % fcount; // Increment the next_frame_ptr
}

// main function
int main(int argc, char **argv) {
    PRA pra; // Page Replacement Algorithm
    char *addrfileName = NULL, *swapfileName = NULL, *outpfileName = NULL; // File names

    char *end;
    int opt, base = 10;
    // Parse the command line arguments
    while ((opt = getopt(argc, argv, "p:r:s:f:a:t:o:")) != -1) {
        switch (opt) {
            case 'p':
                level = (short) strtol(optarg, &end, base);
                if (level < 1 || level > 2) {
                    fprintf(stderr, "Invalid level: %d\n\t1 or 2 expected.\n", level);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'r':
                addrfileName = optarg;
                break;
            case 's':
                swapfileName = optarg;
                break;
            case 'f':
                fcount = (short) strtol(optarg, &end, base);
                if (fcount < 4 || fcount > 128) {
                    fprintf(stderr, "Invalid frame count: %d\n\t4 <= frame count <= 128 expected.\n", fcount);
                    exit(EXIT_FAILURE);
                }

                PHYSICAL_MEM_SIZE = fcount * PAGE_SIZE; // Calculate the physical memory size
                // Allocate the physical memory
                PHYSICAL_MEM = (unsigned char *) malloc(sizeof(unsigned char) * PHYSICAL_MEM_SIZE);
                next_frame_ptr = 0; // Initialize the next_frame_ptr
                break;
            case 'a':
                capitalize(optarg);
                pra = getPRAlgorithm(optarg); // Get the page replacement algorithm
                break;
            case 't':
                tick = (short) strtol(optarg, &end, base);
                tick_counter = 0; // Initialize the tick counter
                break;
            case 'o':
                outpfileName = optarg;
                break;
            default:
                // Print the usage of the program
                fprintf(stderr, "Usage: %s -p <level> -r <address file> -s <swap file> -f <frame count>"
                                "-a <page replacement algorithm> -t <tick> -o <output file>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check if the required arguments are given
    if (level == 0 || addrfileName == NULL || swapfileName == NULL ||
        fcount == 0 || tick == 0 || outpfileName == NULL) {
        // Incorrect usage of the program
        fprintf(stderr, "Missing required arguments.\n");
        exit(EXIT_FAILURE);
    }

    FILE *addrfile, *swapfile, *outpfile;
    // Open the files
    addrfile = fopen(addrfileName, "r+");
    if (addrfile == NULL) {
        free(PHYSICAL_MEM);

        fprintf(stderr, "Cannot open the address file: %s\n", addrfileName);
        exit(EXIT_FAILURE);
    }

    swapfile = fopen(swapfileName, "rb+");
    if (swapfile == NULL) {
        // If the swap file does not exist, create it
        swapfile = fopen(swapfileName, "wb+");
        if (swapfile == NULL) {
            free(PHYSICAL_MEM);
            fclose(addrfile);

            fprintf(stderr, "Cannot open/create the swap file: %s\n", swapfileName);
            exit(EXIT_FAILURE);
        }

        // Write zeros to the swap file if it is just created
        unsigned char zeroBuffer[VIRTUAL_MEM_SIZE] = {0}; // VIRTUAL_MEM_SIZE = 65536 = 2^16 = 1024 * 64
        fwrite(zeroBuffer, sizeof(unsigned char), VIRTUAL_MEM_SIZE, swapfile);
    }

    outpfile = fopen(outpfileName, "w+");
    if (outpfile == NULL) {
        free(PHYSICAL_MEM);
        fclose(addrfile);
        fclose(swapfile);

        fprintf(stderr, "Cannot open/create the output file: %s\n", outpfileName);
        exit(EXIT_FAILURE);
    }

    struct PageTable pt; // Page Table
    initPageTable(&pt, PAGE_COUNT, level); // Initialize the page table

    char line[256];
    unsigned int ln = 0, numberofpgfaults = 0; // line number, number of page faults
    // Read from the address file line by line
    while (fgets(line, sizeof(line), addrfile) != NULL) {
        ln += 1; // increment line number
        char operation; // 'r' or 'w'
        unsigned long logicalAddress, writeValue;
        // read from address file
        const short op = (short) sscanf(line, " %c 0x%lx 0x%lx", &operation, &logicalAddress, &writeValue);
        // op is 2 if operation is 'r', 3 if operation is 'w'

        // initialize variables
        unsigned short pgfault = FALSE; // initialize to FALSE
        unsigned const short VPN = (unsigned short) (logicalAddress >> 6);
        unsigned const short offset = (unsigned short) (logicalAddress & 0x003F);
        unsigned short frameNumber;

        PTE *pte = getPageTableEntry(&pt, VPN);

        if (pte->v == 0) { // page fault
            pgfault = TRUE; // set to TRUE
            numberofpgfaults += 1; // increment number of page faults

            // read from swap file
            switch (pra) {
                case FIFO: {
                    fifo(&pt, VPN, swapfile, &frameNumber);
                    break;
                }
                case LRU: {
                    lru(&pt, VPN, swapfile, &frameNumber);
                    break;
                }
                case CLOCK: {
                    clock(&pt, VPN, swapfile, &frameNumber);
                    break;
                }
                case ECLOCK: {
                    eclock(&pt, VPN, swapfile, &frameNumber);
                    break;
                }
            }

            pte->frame = frameNumber; // set frameNumber
        }

        if (pte->age < 63) {
            pte->age += 1; // increment age
        }

        pte->v = 1; // set vBit to 1, page is in physical memory

        if (pra != FIFO && pra != LRU) {
            pte->r = 1; // set rBit to 1, memory reference is made
            tick_counter += 1; // increment tick counter

            if (tick_counter >= tick) { // == would also work but >= is safer
                clearAllReferencedBits(&pt);
                tick_counter = 0;
            }
        }

        if (op == 2) {
            // read from physical memory (nothing to do here)
            // unsigned const short valueRead = PHYSICAL_MEM[frameNumber * PAGE_SIZE + offset];
        } else if (op == 3) {
            // write to physical memory
            PHYSICAL_MEM[frameNumber * PAGE_SIZE + offset] = (unsigned char) writeValue;

            // update page table, set mBit to 1
            pte->m = 1;
        } else { // if op is not 2 ('r') or 3 ('w'), then it is invalid
            // Close the files
            fclose(addrfile);
            fclose(swapfile);
            fclose(outpfile);

            // Free dynamically allocated memory
            freePageTable(&pt);
            free(PHYSICAL_MEM);

            // Print the error message
            fprintf(stderr, "Invalid line: '%s'\n\tat line %d\n", line, ln);
            exit(EXIT_FAILURE);
        }

        // calculate VPN1 and VPN2
        unsigned const short VPN1 = VPN >> 5, VPN2 = VPN & 0x001F;
        // write to the output file in format of VA PTE1 PTE2 OFFSET PFN PA PF
        fprintf(outpfile, "0x%04lx 0x%03hx 0x%02hx 0x%02hx 0x%04hx 0x%04hx%s\n",
                logicalAddress, (level == 1 ? VPN : VPN1), (level == 1 ? 0 : VPN2), offset,
                frameNumber, (frameNumber * PAGE_SIZE + offset), (pgfault ? " pgfault" : ""));
    }

    // write to the output file the total number of page faults
    fprintf(outpfile, "\nTotal number of page faults: %d\n", numberofpgfaults);

    // write the remaining pages that are in physical memory and modified to swap file before exiting
    unsigned short vpn;
    for (int i = 0; i < fcount; i++) {
        PTE *pte = frameNumberToPageTableEntry(&pt, i, &vpn);

        if (pte != NULL) {
            if (pte->m == 1) {
                fseek(swapfile, vpn * PAGE_SIZE, SEEK_SET);
                fwrite(PHYSICAL_MEM + i * PAGE_SIZE,
                       sizeof(unsigned char), PAGE_SIZE, swapfile);

                pte->m = 0;
            }

            pte->v = 0;
        }
    }

    // Close the files
    fclose(addrfile);
    fclose(swapfile);
    fclose(outpfile);

    // Free dynamically allocated memory
    freePageTable(&pt); // Free the page table
    free(PHYSICAL_MEM); // Free the physical memory

    return 0;
}

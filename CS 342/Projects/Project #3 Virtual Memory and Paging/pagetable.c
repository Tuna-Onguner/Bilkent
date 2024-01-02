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
 * Description: Implementation of the page table.
*/

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct PageTableEntry {
    unsigned short r: 1;             // Referenced bit
    unsigned short m: 1;             // Modified/Dirty bit
    unsigned short v: 1;             // Valid/Invalid bit
    unsigned short age: 6;           // Age bits for LRU algorithm
    unsigned short frame: 7;         // Frame number: 2^7 = 128 frames (max)
    // In total, 16 bits are used for a page table entry
} PTE;

struct PageTable {
    PTE *pageTableEntries;
    struct PageTable *innerPageTables;
    unsigned short size, level;
};

void initPageTable(struct PageTable *pt, unsigned const short SIZE, unsigned const short LEVEL) {
    pt->level = LEVEL;
    if (pt->level == 1) {
        pt->size = SIZE;
        pt->pageTableEntries = (PTE *) malloc(sizeof(PTE) * pt->size);
        pt->innerPageTables = NULL;
    } else if (pt->level == 2) {
        pt->size = (short) sqrt(SIZE);
        pt->pageTableEntries = NULL;
        pt->innerPageTables = (struct PageTable *) malloc(sizeof(struct PageTable) * pt->size);
        for (int i = 0; i < pt->size; i++) {
            initPageTable(&pt->innerPageTables[i], pt->size, 1);
        }
    }
}

PTE *getPageTableEntry(struct PageTable *pt, unsigned const short VPN) {
    if (pt->level == 1) {
        return &pt->pageTableEntries[VPN];
    } else if (pt->level == 2) {
        unsigned const short VPN1 = VPN >> 5, VPN2 = VPN & 0x001F;
        return &pt->innerPageTables[VPN1].pageTableEntries[VPN2];
    }

    return NULL;
}

PTE *frameNumberToPageTableEntry(struct PageTable *pt, unsigned const short frameNumber, unsigned short *vpn) {
    if (pt->level == 1) {
        for (int i = 0; i < pt->size; i++) {
            if (pt->pageTableEntries[i].frame == frameNumber && pt->pageTableEntries[i].v == 1) {
                *vpn = (unsigned short) i;
                return &pt->pageTableEntries[i];
            }
        }
    } else if (pt->level == 2) {
        for (int i = 0; i < pt->size; i++) {
            for (int j = 0; j < pt->innerPageTables[i].size; j++) {
                if (pt->innerPageTables[i].pageTableEntries[j].v == 1 &&
                    pt->innerPageTables[i].pageTableEntries[j].frame == frameNumber) {
                    *vpn = (unsigned short) i * pt->innerPageTables[i].size + j;
                    return &pt->innerPageTables[i].pageTableEntries[j];
                }
            }
        }
    }

    return NULL;
}

void clearAllReferencedBits(struct PageTable *pt) {
    if (pt->level == 1) {
        for (int i = 0; i < pt->size; i++) {
            pt->pageTableEntries[i].r = 0;
        }
    } else if (pt->level == 2) {
        for (int i = 0; i < pt->size; i++) {
            clearAllReferencedBits(&pt->innerPageTables[i]);
        }
    }
}

void freePageTable(struct PageTable *pt) {
    if (pt->level == 1) {
        free(pt->pageTableEntries);
        pt->pageTableEntries = NULL;
    } else if (pt->level == 2) {
        for (int i = 0; i < pt->size; i++) {
            freePageTable(&pt->innerPageTables[i]);
        }

        free(pt->innerPageTables);
        pt->innerPageTables = NULL;
    }
}

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
 * Description: Functionality for creating and formatting a disk.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vsfs.h"

int main(int argc, char **argv) {
    int ret, m;
    char vdiskname[200];

    if (argc != 3) {
        printf("usage: create_format <vdiskname> <m>\n");
        exit(1);
    }

    strcpy(vdiskname, argv[1]);
    m = atoi(argv[2]);

    printf("started\n");

    ret = vsformat(vdiskname, m);
    if (ret != 0) {
        printf("there was an error in creating the disk\n");
        exit(1);
    }

    printf("disk created and formatted. %s %d\n", vdiskname, m);

    return 0;
}

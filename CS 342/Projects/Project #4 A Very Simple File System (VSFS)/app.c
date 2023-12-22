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
 * Description: Application file for the VSFS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vsfs.h"

int main(int argc, char **argv) {
    int ret;
    int fd1, fd2, fd;
    int i;
    char c;
    char buffer[2048];
    char buffer2[8] = {50, 50, 50, 50, 50, 50, 50, 50};
    int size;
    char vdiskname[200];

    printf("started\n");

    if (argc != 2) {
        printf("usage: app  <vdiskname>\n");
        exit(0);
    }

    strcpy(vdiskname, argv[1]);

    ret = vsmount(vdiskname);
    if (ret != 0) {
        printf("could not mount \n");
        exit(1);
    }

    printf("creating files\n");

    vscreate("file1.bin");
    vscreate("file2.bin");
    vscreate("file3.bin");

    fd1 = vsopen("file1.bin", MODE_APPEND);
    fd2 = vsopen("file2.bin", MODE_APPEND);

    for (i = 0; i < 10000; ++i) {
        buffer[0] = (char) 65;
        vsappend(fd1, (void *) buffer, 1);
    }

    for (i = 0; i < 1000; ++i) {
        buffer[0] = (char) 65;
        buffer[1] = (char) 66;
        buffer[2] = (char) 67;
        buffer[3] = (char) 68;
        vsappend(fd2, (void *) buffer, 4);
    }

    vsclose(fd1);
    vsclose(fd2);

    fd = vsopen("file3.bin", MODE_APPEND);
    for (i = 0; i < 1000; ++i) {
        memcpy(buffer, buffer2, 8); // just to show memcpy
        vsappend(fd, (void *) buffer, 8);
    }

    vsclose(fd);

    fd = vsopen("file3.bin", MODE_READ);

    size = vssize(fd);
    for (i = 0; i < size; ++i) {
        vsread(fd, (void *) buffer, 1);
        c = (char) buffer[0];
        c = c + 1; // just to do something
    }

    vsclose(fd);
    vsumount();

    printf("finished\n");

    return 0;
}

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
 * Description: This is the header file for the VSFS.
 */


#ifndef VSFS_H
#define VSFS_H

#define MODE_READ   0
#define MODE_APPEND 1
#define BLOCKSIZE   2048 // bytes

int vsformat(char *vdiskname, int m);

int vsmount(char *vdiskname);

int vsumount();

int vscreate(char *filename);

int vsopen(char *filename, int mode);

int vsclose(int fd);

int vssize(int fd);

int vsread(int fd, void *buf, int n);

int vsappend(int fd, void *buf, int n);

int vsdelete(char *filename);

#endif //VSFS_H

#
# Bilkent University
# Semester of Fall 2023
#
# Department of Computer Engineering
#
# CS342 Operating Systems
# Project #4: A Very Simple File System (VSFS)
#
# Section 1
# Instructor: İbrahim Körpeoğlu
#
# Authors: Deniz Tuna Onguner (22001788) and
#          Sarper Arda Bakır (21902781)
#
# version 1.0
# Date: 20 December 2023, Wednesday
#
# Description: This is the Makefile for the VSFS project.
#

all: libvsfs.a create_format app


libvsfs.a: vsfs.c
	gcc -Wall -c vsfs.c
	ar -cvq libvsfs.a vsfs.o
	ranlib libvsfs.a


create_format: create_format.c
	gcc -Wall -o create_format create_format.c -L. -lvsfs


app: app.c
	gcc -Wall -o app app.c -L. -lvsfs


clean:
	rm -f *.o *.a app vdisk create_format

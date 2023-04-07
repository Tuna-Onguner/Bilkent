//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201, Section 1
// Homework 3
// 27.04.2022
// Bilkent University
//

#ifndef _PUZZLEROW_H
#define _PUZZLEROW_H

#include <iostream>

using namespace std;

class PuzzleRow {
private:
    struct PuzzlePiece {
        char shape;
        int rowindex;
        int colindex;
        PuzzlePiece *next;
    };
    PuzzlePiece *head;
    PuzzleRow *next;
    int rowindex;
    int maxSize;
    int size;

    PuzzlePiece *findPiece(int index) const;

public:
    PuzzleRow(int size, int rowindex);

    ~PuzzleRow();

    int getRowIndex() const;

    void setNext(PuzzleRow *next);

    PuzzleRow *getNext() const;

    bool insert(int index, char shape);

    bool remove(int index);

    bool isEmpty() const;

    char getPiece(int index) const;

    void printRow() const;

    bool isOccupied(int index) const;
};


#endif //_PUZZLEROW_H

//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201, Section 1
// Homework 3
// 27.04.2022
// Bilkent University
//

#ifndef _PUZZLECONTAINER_H
#define _PUZZLECONTAINER_H

#include "PuzzleRow.h"

namespace {
    void shapeConverter(const unsigned char shape, int &down, int &up, int &right, int &left) {
        down = shape % 4;
        up = (shape / 4) % 4;
        right = (shape / 16) % 4;
        left = shape / 64;
    }
}

class PuzzleContainer {
private:
    PuzzleRow *rowList;
    int height;
    int width;
    int size;

    PuzzleRow *findRow(int index) const;

    bool isFitInto(PuzzleRow *theRow, int row, int col, char shape) const;

    void shapeToPuzzle(int down, int up, int right, int left, string &d, string &u, string &r, string &l) const;

public:
    PuzzleContainer(int h, int w);

    ~PuzzleContainer();

    PuzzleRow *getRow(int index) const;

    bool insertRow(int index);

    bool removeRow(int index);

    bool addPiece(int row, int col, char shape);

    bool removePiece(int row, int col);

    int getHeight() const;

    int getWidth() const;

    void showPiece(int row, int col) const;

    void displayContainer() const;

    void showColumn(int col) const;

    void showRow(int row) const;

    void findPiece(char shape) const;
};


#endif //_PUZZLECONTAINER_H

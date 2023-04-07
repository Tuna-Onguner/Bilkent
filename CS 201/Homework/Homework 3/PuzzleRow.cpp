//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201, Section 1
// Homework 3
// 27.04.2022
// Bilkent University
//

#include "PuzzleRow.h"

PuzzleRow::PuzzleRow(const int maxsize, const int row) : head(NULL), next(NULL), size(0) {
    if (maxsize > 0 && row > 0) {
        maxSize = maxsize;
        rowindex = row;
    }
}

PuzzleRow::~PuzzleRow() {
    for (int i = 1; i <= maxSize; i++)
        remove(i);
}

int PuzzleRow::getRowIndex() const {
    return rowindex;
}

void PuzzleRow::setNext(PuzzleRow *newnext) {
    next = newnext;
}

PuzzleRow *PuzzleRow::getNext() const {
    return next;
}

bool PuzzleRow::insert(int index, const char shape) {
    if (index < 1 || index > maxSize || isOccupied(index))
        return false;
    else {
        const int constindex = index;
        size += 1;
        index = 1;
        PuzzlePiece *newpiece = new PuzzlePiece;
        newpiece->shape = shape;
        newpiece->colindex = constindex;
        newpiece->rowindex = rowindex;
        if (head != NULL) {
            PuzzlePiece *cur = head;
            for (int i = 1; i < size && cur->colindex <= constindex; i++, cur = cur->next)
                index += 1;
        }
        if (index == 1) {
            newpiece->next = head;
            head = newpiece;
        } else {
            PuzzlePiece *prev = findPiece(index - 1);
            newpiece->next = prev->next;
            prev->next = newpiece;
        }
        std::cout << "A piece is added to (" << rowindex << ',' << constindex << ')' << std::endl;
        return true;
    }
}

bool PuzzleRow::remove(int index) {
    if (head == NULL || index < 1 || index > maxSize)
        return false;
    else {
        const int constindex = index;
        index = 1;
        PuzzlePiece *cur = head;
        for (int i = 1; i < size && cur->colindex != constindex; i++, cur = cur->next)
            index += 1;
        size -= 1;
        if (index == 1) {
            cur = head;
            head = head->next;
        } else {
            PuzzlePiece *prev = findPiece(index - 1);
            cur = prev->next;
            prev->next = cur->next;
        }
        cur->next = NULL;
        delete cur;
        cur = NULL;
        return true;
    }
}

bool PuzzleRow::isEmpty() const {
    return size == 0;
}

char PuzzleRow::getPiece(const int index) const {
    if (index < 1 || index > maxSize || head == NULL)
        return 0xFF;
    else {
        PuzzleRow::PuzzlePiece *cur = head;
        for (int i = 1; i <= maxSize && cur != NULL; i++, cur = cur->next) {
            if (cur->colindex == index)
                return cur->shape;
        }
        return 0xFF;
    }
}

PuzzleRow::PuzzlePiece *PuzzleRow::findPiece(const int index) const {
    if (index < 1 || index > size + 1 || index > maxSize)
        return NULL;
    else {
        PuzzlePiece *cur = head;
        for (int skp = 1; skp < index; skp++)
            cur = cur->next;
        return cur;
    }
}

void PuzzleRow::printRow() const {
    if (head != NULL) {
        PuzzleRow::PuzzlePiece *cur = head;
        for (int i = 1; i <= maxSize; i++)
            if (cur != NULL && cur->colindex == i) {
                std::cout << "x ";
                cur = cur->next;
            } else
                std::cout << "o ";
    } else
        for (int i = 1; i <= maxSize; i++)
            std::cout << "o ";
    std::cout << std::endl;
}

bool PuzzleRow::isOccupied(const int index) const {
    PuzzlePiece *cur = head;
    for (int i = 1; i <= size; i++, cur = cur->next) {
        if (cur->colindex == index) {
            std::cout << "(" << rowindex << "," << index << ") has been already occupied, it cannot be added"
                      << std::endl;
            return true;
        }
    }
    return false;
}

//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201, Section 1
// Homework 3
// 27.04.2022
// Bilkent University
//

#include "PuzzleContainer.h"

PuzzleContainer::PuzzleContainer(const int h, const int w) : rowList(NULL), size(0) {
    if (h > 0 && w > 0) {
        height = h;
        width = w;
    }
}

PuzzleContainer::~PuzzleContainer() {
    for (int i = 1; i <= getHeight(); i++)
        removeRow(i);
}

PuzzleRow *PuzzleContainer::getRow(const int index) const {
    if (index < 1 || index > height || rowList == NULL)
        return NULL;
    else {
        PuzzleRow *cur = rowList;
        for (int j = 1; j <= height && cur != NULL; j++, cur = cur->getNext())
            if (cur->getRowIndex() == index)
                return cur;
        return NULL;
    }
}

PuzzleRow *PuzzleContainer::findRow(const int index) const {
    if (index > height || index < 1 || index > size + 1 || rowList == NULL)
        return NULL;
    else {
        PuzzleRow *cur = rowList;
        for (int skp = 1; skp < index && cur != NULL; skp++)
            cur = cur->getNext();
        return cur;
    }
}

bool PuzzleContainer::insertRow(int index) {
    if (index > height || index < 1)
        return false;
    else {
        const int constindex = index;
        size += 1;
        index = 1;
        if (rowList != NULL) {
            PuzzleRow *cur = rowList;
            for (int i = 1; i < size && cur->getRowIndex() <= constindex; i++, cur = cur->getNext())
                index += 1;
        }
        PuzzleRow *newRow = new PuzzleRow(getWidth(), constindex);
        if (index == 1) {
            newRow->setNext(rowList);
            rowList = newRow;
        } else {
            PuzzleRow *prev = findRow(index - 1);
            newRow->setNext(prev->getNext());
            prev->setNext(newRow);
        }
        return true;
    }
}

bool PuzzleContainer::removeRow(int index) {
    if (rowList == NULL || index < 1 || index > height)
        return false;
    else {
        const int constindex = index;
        index = 1;
        PuzzleRow *cur = rowList;
        for (int i = 1; i < size && cur->getRowIndex() < constindex; i++, cur = cur->getNext())
            index += 1;
        size -= 1;
        if (index == 1) {
            cur = rowList;
            rowList = rowList->getNext();
        } else {
            PuzzleRow *prev = findRow(index - 1);
            cur = prev->getNext();
            prev->setNext(cur->getNext());
        }
        cur->setNext(NULL);
        delete cur;
        cur = NULL;
        return true;
    }
}

bool PuzzleContainer::addPiece(const int row, const int col, const char shape) {
    if (row > 0 && row <= height && col > 0 && col <= width) {
        PuzzleRow *theRow = getRow(row);
        if (theRow == NULL) {
            insertRow(row);
            theRow = getRow(row);
        }
        if (!isFitInto(theRow, row, col, shape)) {
            std::cout << "Shape doesn't fit into " << '(' << row << ',' << col << "), it cannot be added" << std::endl;
            if (theRow->isEmpty())
                removeRow(row);
            return false;
        } else {
            theRow->insert(col, shape);
            return true;
        }
    } else {
        std::cout << '(' << row << ',' << col << ") is out of frame, it cannot be added" << std::endl;
        return false;
    }
}

bool PuzzleContainer::removePiece(const int row, const int col) {
    if (row > 0 && row <= height && col > 0 && col <= width) {
        PuzzleRow *theRow = getRow(row);
        if (theRow == NULL) {
            std::cout << "There is no piece on (" << row << ',' << col << "), it cannot be removed" << std::endl;
            return false;
        } else {
            if (theRow->getPiece(col) == (char) 0xFF) {
                std::cout << "There is no piece on (" << row << ',' << col << "), it cannot be removed" << std::endl;
                return false;
            } else {
                std::cout << "The piece on (" << row << ',' << col << ") is removed" << std::endl;
                theRow->remove(col);
                if (theRow->isEmpty())
                    removeRow(row);
                return true;
            }
        }
    } else {
        std::cout << '(' << row << ',' << col << ") is out of frame, it cannot be removed" << std::endl;
        return false;
    }
}

int PuzzleContainer::getHeight() const {
    return height;
}

int PuzzleContainer::getWidth() const {
    return width;
}

void PuzzleContainer::showPiece(const int row, const int col) const {
    if (row > 0 && row <= height && col > 0 && col <= width) {
        PuzzleRow *theRow = getRow(row);
        if (theRow == NULL)
            std::cout << "There is no piece on (" << row << ',' << col << "), it cannot be shown" << std::endl;
        else {
            int piece = theRow->getPiece(col);
            if (piece == (char) 0xFF)
                std::cout << "There is no piece on (" << row << ',' << col << "), it cannot be shown" << std::endl;
            else {
                string d, u, r, l;
                int down, up, right, left;
                shapeConverter(piece, down, up, right, left);
                shapeToPuzzle(down, up, right, left, d, u, r, l);
                std::cout << "Piece on (" << row << ',' << col << ") as following:" << std::endl << "Down: " << d
                          << std::endl << "Up: " << u << std::endl << "Right: " << r << std::endl << "Left: "
                          << l << std::endl;
            }
        }
    } else
        std::cout << '(' << row << ',' << col << ") is out of frame, it cannot be shown" << std::endl;
}

void PuzzleContainer::displayContainer() const {
    std::cout << "Container " << height << 'x' << width << ':' << std::endl << "  ";
    for (int i = 1; i <= width; i++) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    if (rowList != NULL) {
        PuzzleRow *row;
        for (int i = 1; i <= height; i++) {
            row = getRow(i);
            std::cout << i << ' ';
            if (row != NULL)
                row->printRow();
            else {
                for (int j = 1; j <= width; j++)
                    std::cout << "o ";
                std::cout << std::endl;
            }
        }
    } else {
        for (int i = 1; i <= height; i++) {
            std::cout << i << ' ';
            for (int j = 1; j <= width; j++) {
                std::cout << "o ";
            }
            std::cout << std::endl;
        }
    }
}

void PuzzleContainer::showColumn(const int col) const {
    if (col > width || col < 1)
        std::cout << "Col " << col << " is out of border" << std::endl;
    else {
        std::cout << "Col " << col << ':' << std::endl;
        int piece;
        PuzzleRow *Row;
        for (int i = 1; i <= height; i++) {
            Row = getRow(i);
            if (Row != NULL) {
                piece = Row->getPiece(col);
                if (piece != (char) 0xFF)
                    std::cout << 'x' << std::endl;
                else
                    std::cout << 'o' << std::endl;
            } else
                std::cout << 'o' << std::endl;
        }
    }
}

void PuzzleContainer::showRow(const int row) const {
    if (row > height || row < 1)
        std::cout << "Row " << row << " is out of border" << std::endl;
    else {
        PuzzleRow *Row = getRow(row);
        std::cout << "Row " << row << ':' << std::endl;
        if (Row != NULL) {
            Row->printRow();
        } else {
            for (int i = 1; i <= width; i++)
                std::cout << "o ";
            std::cout << std::endl;
        }
    }
}

void PuzzleContainer::findPiece(const char shape) const {
    PuzzleRow *row;
    int piece, count = 0;
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {
            row = getRow(i);
            if (row != NULL) {
                piece = row->getPiece(j);
                if (piece == shape) {
                    if (count >= 1)
                        std::cout << ", ";
                    count += 1;
                    std::cout << '(' << i << ',' << j << ")";
                }
            }
        }
    }
    if (count == 0)
        std::cout << "There is no piece that has shape" << std::endl;
    else
        std::cout << " contain the piece that has shape" << std::endl;
    int down, up, right, left;
    string d, u, r, l;
    shapeConverter(shape, down, up, right, left);
    shapeToPuzzle(down, up, right, left, d, u, r, l);
    std::cout << "Down: " << d << std::endl << "Up: " << u << std::endl << "Right: " << r << std::endl << "Left: "
              << l << std::endl;
}

bool PuzzleContainer::isFitInto(PuzzleRow *theRow, const int row, const int col, const char shape) const {
    int down, up, right, left;
    shapeConverter(shape, down, up, right, left);
    if ((row == 1 && up != 2) || (row == height && down != 2) ||
        (col == 1 && left != 2) || (col == width && right != 2)) {
        return false;
    } else {
        PuzzleRow *dRow, *uRow;
        int upPiece, downPiece, dFit, uFit, rFit, lFit, rl, lr, garbage;
        dRow = getRow(row + 1);
        uRow = getRow(row - 1);
        rl = theRow->getPiece(col + 1);
        lr = theRow->getPiece(col - 1);
        if (dRow == NULL) {
            dFit = -1;
        } else {
            downPiece = dRow->getPiece(col);
            if (downPiece == (char) 0xFF)
                dFit = -1;
            else
                shapeConverter(downPiece, garbage, dFit, garbage, garbage);
        }
        if (uRow == NULL) {
            uFit = -1;
        } else {
            upPiece = uRow->getPiece(col);
            if (upPiece == (char) 0xFF)
                uFit = -1;
            else
                shapeConverter(upPiece, uFit, garbage, garbage, garbage);
        }
        if (rl == (char) 0xFF) {
            rFit = -1;
        } else {
            shapeConverter(rl, garbage, garbage, garbage, rFit);
        }
        if (lr == (char) 0xFF) {
            lFit = -1;
        } else {
            shapeConverter(lr, garbage, garbage, lFit, garbage);
        }
        if (((dFit == -1) || (down + dFit == 1)) && ((uFit == -1) || (up + uFit == 1)) &&
            ((rFit == -1) || (right + rFit == 1)) && ((lFit == -1) || (left + lFit == 1))) {
            return true;
        }
        return false;
    }
}

void PuzzleContainer::shapeToPuzzle(const int down, const int up, const int right, const int left,
                                    string &d, string &u, string &r, string &l) const {
    switch (down) {
        case 0:
            d = "Hole";
            break;
        case 1:
            d = "Knob";
            break;
        case 2:
            d = "Border";
            break;
        default:
            d = "";
    }
    switch (up) {
        case 0:
            u = "Hole";
            break;
        case 1:
            u = "Knob";
            break;
        case 2:
            u = "Border";
            break;
        default:
            u = "";
    }
    switch (right) {
        case 0:
            r = "Hole";
            break;
        case 1:
            r = "Knob";
            break;
        case 2:
            r = "Border";
            break;
        default:
            r = "";
    }
    switch (left) {
        case 0:
            l = "Hole";
            break;
        case 1:
            l = "Knob";
            break;
        case 2:
            l = "Border";
            break;
        default:
            l = "";
    }
}

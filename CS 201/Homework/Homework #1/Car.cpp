//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201 - Section 1
// Homework 1
// 22.03.2022
// Spring 2021/2022
// Bilkent University
//

#include "Car.h"

Car::Car(int pn, int rw, int cl, int ct, int li) {
    plateNumber = pn;
    row = rw;
    col = cl;
    carType = ct;
    lotId = li;
}

Car &Car::operator=(Car &right) {
    if (&right != this) {
        plateNumber = right.plateNumber;
        row = right.row;
        col = right.col;
        carType = right.carType;
        lotId = right.lotId;
    }
    return *this;
}

int Car::getPlateNumber() const {
    return plateNumber;
}

int Car::getRow() const {
    return row;
}

int Car::getCol() const {
    return col;
}

int Car::getCarType() const {
    return carType;
}

int Car::getLotId() const {
    return lotId;
}

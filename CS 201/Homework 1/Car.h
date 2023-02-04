//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201 - Section 1
// Homework 1
// 22.03.2022
// Spring 2021/2022
// Bilkent University
//

#ifndef _CAR_H
#define _CAR_H

class Car {
public:
    explicit Car(int = 0, int = 0, int = 0, int = 0, int = 0);

    Car &operator=(Car &);

    int getPlateNumber() const;

    int getRow() const;

    int getCol() const;

    int getCarType() const;

    int getLotId() const;

private:
    int plateNumber;
    int row;
    int col;
    int carType;
    int lotId;
};

#endif //_CAR_H

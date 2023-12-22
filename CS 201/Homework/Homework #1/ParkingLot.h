//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201 - Section 1
// Homework 1
// 22.03.2022
// Spring 2021/2022
// Bilkent University
//

#ifndef _PARKINGLOT_H
#define _PARKINGLOT_H

#include <iostream>
using namespace std;

class ParkingLot {
private:
    int ID;
    int row;
    int col;
    int *parkedCars;
    char **arr;

    void fillInitialLot() const;

public:
    explicit ParkingLot(int = 0, int = 0, int = 0);

    ~ParkingLot();

    ParkingLot &operator=(const ParkingLot &);

    void printParkingLot();

    void coutLotInfo() const;

    void park_car(int) const;

    void remove_car(int) const;

    bool isOccupied(const string &, const int, const int) const;

    int numberEmptyPlaces() const;

    int getId() const;

    int getArea() const;

    char **getArr() const;
};

#endif //_PARKINGLOT_H

//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201 - Section 1
// Homework 1
// 22.03.2022
// Spring 2021/2022
// Bilkent University
//

#ifndef _PARKINGSYSTEM_H
#define _PARKINGSYSTEM_H

#include "ParkingLot.h"
#include "Car.h"

class ParkingSystem {
public:
    ParkingSystem();

    ~ParkingSystem();

    void createLot(int id, int rows, int columns);

    void removeLot(int id);

    void listLots();

    void lotContents(int id);

    void parkCar(int lotId, string location, string carType, int plateNumber);

    void findCar(int plateNumber);

    void removeCar(int plateNumber);

private:
    int lotArrSize;
    int carInfoSize;
    ParkingLot *parkingLotArr;
    Car *carInfoList;

    bool searchParkingLotArr(const int, const ParkingLot *, const int) const;

    bool searchCars(const int, const Car *, const int) const;
};

#endif //_PARKINGSYSTEM_H

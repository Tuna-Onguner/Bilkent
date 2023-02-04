//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201 - Section 1
// Homework 1
// 22.03.2022
// Spring 2021/2022
// Bilkent University
//

#include "ParkingSystem.h"

// Constructor
ParkingSystem::ParkingSystem() {
    lotArrSize = 0;
    carInfoSize = 0;
    parkingLotArr = NULL;
    carInfoList = NULL;
}

// Destructor
ParkingSystem::~ParkingSystem() {
    delete[] parkingLotArr;
    delete[] carInfoList;
}

/**
 * createLot(): void
 * Creates a new parking lot object according to the user inputs.
 * If the user enters no value, then every value is initialized to 0 as default.
 * If invalid values are passed, then an error message is displayed.
 * If a parking lot with the passed id value already exists, then an error message is displayed.
 * New created parking lot is added into the related array.
 * @param id
 * @param rows
 * @param columns
 */
void ParkingSystem::createLot(const int id, const int rows, const int columns) {
    if (!searchParkingLotArr(id, parkingLotArr, lotArrSize)) {
        if (rows > 0 && rows <= 12 && columns > 0 && columns <= 12) {
            // Adding new created parking lot into the parking lot list
            ParkingLot *newLot = new ParkingLot(id, rows, columns);
            ParkingLot *tempArr = parkingLotArr;
            parkingLotArr = new ParkingLot[lotArrSize + 1];
            if (tempArr != NULL) {
                for (int i = 0; i < lotArrSize; i++) {
                    parkingLotArr[i] = tempArr[i];
                }
            }
            parkingLotArr[lotArrSize] = *newLot;
            lotArrSize++;
            // Printing the related massage
            cout << "Parking lot with ID " << id << " and dimensions ("
                 << rows << "," << columns << ") has been added to the system" << endl;
            // Deleting the temporary objects and arrays
            delete newLot;
            delete[] tempArr;
        } else
            cout << "Cannot create the parking lot " << id << ", dimensions exceed acceptable bounds" << endl;
    } else
        cout << "Cannot create the parking lot " << id << ", a lot with this ID already exists" << endl;
}

/**
 * removeLot(): void
 * From the parking lot array, the lot with the passed id number is removed.
 * If there are parked cars in the related lot, then first these cars are removed.
 * If the passed id number is not in the array, then an error message is displayed.
 * @param id
 */
void ParkingSystem::removeLot(const int id) {
    if (searchParkingLotArr(id, parkingLotArr, lotArrSize)) {
        for (int i = 0; i < lotArrSize; i++) {
            if (parkingLotArr[i].getId() == id) {
                // Removing cars from the lot
                if (parkingLotArr[i].getArea() != parkingLotArr[i].numberEmptyPlaces()) {
                    for (int j = 0; j < carInfoSize; j++)
                        if (carInfoList[j].getLotId() == id)
                            removeCar(carInfoList[j].getPlateNumber());
                }
                // Removing the lot
                ParkingLot *tempArr = parkingLotArr;
                parkingLotArr = new ParkingLot[lotArrSize - 1];
                for (int j = 0; j < lotArrSize - 1; j++) {
                    if (tempArr[j].getId() != id)
                        parkingLotArr[j] = tempArr[j];
                    else
                        parkingLotArr[j] = tempArr[lotArrSize - 1];
                }
                lotArrSize--;
                // Printing the related massage
                cout << "Lot " << id << " has been successfully removed from the system" << endl;
                // Deleting the temporary array
                delete[] tempArr;
                break;
            }
        }
    } else
        cout << "Lot " << id << " is not in the system" << endl;
}

/**
 * ListLots(): void
 * Lists all the existing parking lots and their info in the lot array.
 * If the array is empty, no existing parking lots; then a message, saying no lots are possible, is printed.
 */
void ParkingSystem::listLots() {
    if (parkingLotArr != NULL) {
        int counter = lotArrSize;
        if (lotArrSize > 0) {
            cout << "List of lots:" << endl;
            for (int i = 0; i < lotArrSize; i++) {
                if (parkingLotArr[i].getId() != 0) {
                    parkingLotArr[i].coutLotInfo();
                    counter--;
                }
            }
        } else
            cout << "No lots to list" << endl;
    } else
        cout << "No lots to list" << endl;
}

/**
 * LotContents(): void
 * Prints the contents of the lot whose id is passed.
 * Contents are the locations of the cars that are in the lot, and the ids of the lots.
 * @param id
 */
void ParkingSystem::lotContents(const int id) {
    for (int i = 0; i < lotArrSize; i++) {
        if (parkingLotArr[i].getId() == id) {
            parkingLotArr[i].printParkingLot();
            break;
        }
    }
}

/**
 * parkCar(): void
 * Parks a car into the lot whose id is passed.
 * If such a car with the passed id already exists, then a related message is displayed.
 * If parking is successful, then a related message is displayed.
 * @param lotId
 * @param location
 * @param carType
 * @param plateNumber
 */
void ParkingSystem::parkCar(const int lotId, const string location, const string carType, const int plateNumber) {
    if (!searchParkingLotArr(lotId, parkingLotArr, lotArrSize)) {
        cout << "Lot " << lotId << " is not in the system" << endl;
    } else {
        if (!searchCars(plateNumber, carInfoList, carInfoSize)) {
            int row  = static_cast<int>(location[0]) - 64;
            int col0 = static_cast<int>(location[1]);
            int col1 = static_cast<int>(location[2]);
            int col;
            if (col1 > 0)
                col = col0 + col1 - 87;
            else
                col = col0 - 48;

            if (carType == "b" && row == 1) {
                cout << "Cannot park heavy-duty car with number plate "
                     << plateNumber << " at location " << location << ", not enough space" << endl;
            } else {
                for (int i = 0; i < lotArrSize; i++) {
                    if (parkingLotArr[i].getId() == lotId) {
                        if (!parkingLotArr[i].isOccupied(carType, row, col)) {
                            parkingLotArr[i].park_car(plateNumber);
                            Car *temp = carInfoList;
                            Car *newCar = new Car(plateNumber, row, col, (carType == "a"), lotId);
                            carInfoList = new Car[carInfoSize + 1];
                            if (temp != NULL) {
                                for (int j = 0; j < carInfoSize; j++) {
                                    carInfoList[j] = temp[j];
                                }
                            }
                            carInfoList[carInfoSize] = *newCar;
                            carInfoSize++;
                            if (carType == "a") {
                                parkingLotArr[i].getArr()[row][col] = 'a';
                                cout << "Light-duty car with number plate " << plateNumber
                                     << " has been parked at location " << location << ", lot " << lotId << endl;
                            } else if (carType == "b") {
                                parkingLotArr[i].getArr()[row][col] = 'b';
                                parkingLotArr[i].getArr()[row - 1][col] = 'b';
                                cout << "Heavy-duty car with number plate " << plateNumber
                                     << " has been parked at location " << location << ", lot " << lotId << endl;
                            }
                            delete newCar;
                            delete[] temp;
                        } else {
                            cout << "Cannot park ";
                            if (carType == "a")
                                cout << "light-duty " << "car with plate number " << plateNumber
                                     << " at location " << location << ", already occupied" << endl;
                            else if (carType == "b")
                                cout << "heavy-duty " << "car with plate number " << plateNumber
                                     << " at location " << location << ", already occupied" << endl;
                        }
                        break;
                    }
                }
            }
        } else
            cout << "Cannot park the car " << plateNumber << ", a car with this plate number already exists" << endl;
    }
}

/**
 * findCar(): void
 * Finds the car that is in the system, according to the passed plate number and prints the location of this car.
 * If a car with the entered id does does not exist in the system, then an error message is displayed.
 * @param plateNumber
 */
void ParkingSystem::findCar(const int plateNumber) {
    if (searchCars(plateNumber, carInfoList, carInfoSize)) {
        for (int i = 0; i < carInfoSize; i++) {
            if (carInfoList[i].getPlateNumber() == plateNumber) {
                if (carInfoList[i].getPlateNumber() == plateNumber) {
                    if (carInfoList[i].getCarType())
                        cout << "Light-duty ";
                    else
                        cout << "Heavy-duty ";
                    cout << "car with number plate " << carInfoList[i].getPlateNumber() << " is at location "
                         << static_cast<char>(carInfoList[i].getRow() + 64) << carInfoList[i].getCol()
                         << ", lot " << carInfoList[i].getLotId() << endl;
                    break;
                }
            }
        }
    } else
        cout << "The car with number plate " << plateNumber << " is not in the system" << endl;
}

/**
 * removeCar(): void
 * Removes the car from the located lot if such a car exists, according to the passed plate number.
 * If the removing is successful, then an error message is displayed.
 * @param plateNumber
 */
void ParkingSystem::removeCar(const int plateNumber) {
    if (plateNumber > 0) {
        if (searchCars(plateNumber, carInfoList, carInfoSize)) {
            int row = 0;
            int col = 0;
            int ctp = 0;
            int lot = 0;
            for (int i = 0; i < carInfoSize; i++) {
                if (carInfoList[i].getPlateNumber() == plateNumber) {
                    row = carInfoList[i].getRow();
                    col = carInfoList[i].getCol();
                    ctp = carInfoList[i].getCarType();
                    lot = carInfoList[i].getLotId();
                    Car *emptyCar = new Car();
                    carInfoList[i] = *emptyCar;
                    Car *temp = carInfoList;
                    carInfoList = new Car[carInfoSize - 1];
                    for (int j = 0; j < carInfoSize - 1; j++) {
                        if (temp[j].getPlateNumber() != 0)
                            carInfoList[j] = temp[j];
                        else
                            carInfoList[j] = temp[carInfoSize - 1];
                    }
                    carInfoSize--;
                    delete emptyCar;
                    delete[] temp;
                    break;
                }
            }
            for (int i = 0; i < lotArrSize; i++) {
                if (parkingLotArr[i].getId() == lot) {
                    parkingLotArr[i].remove_car(plateNumber);
                    parkingLotArr[i].getArr()[row][col] = '+';
                    if (!ctp) {
                        parkingLotArr[i].getArr()[row - 1][col] = '+';
                        cout << "Heavy-duty ";
                    } else
                        cout << "Light-duty ";
                    cout << "car with plate number " << plateNumber << " car has been removed from slot "
                         << static_cast<char>(row + 64) << col << ", lot " << lot << endl;
                    break;
                }
            }
        } else
            cout << "The car with number plate " << plateNumber << " is not in the system" << endl;
    }
}

bool ParkingSystem::searchParkingLotArr(const int target, const ParkingLot *arr, const int size) const {
    if (arr == NULL)
        return 0;
    for (int i = 0; i < size; i++)
        if (arr[i].getId() == target)
            return 1;
    return 0;
}

bool ParkingSystem::searchCars(const int target, const Car *arr, const int size) const {
    if (arr == NULL)
        return 0;
    for (int i = 0; i < size; i++)
        if (arr[i].getPlateNumber() == target)
            return 1;
    return 0;
}

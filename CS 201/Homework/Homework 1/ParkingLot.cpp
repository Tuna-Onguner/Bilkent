//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201 - Section 1
// Homework 1
// 22.03.2022
// Spring 2021/2022
// Bilkent University
//

#include "ParkingLot.h"

ParkingLot::ParkingLot(const int aID, const int aRow, const int aCol) {
    ID  = aID;
    row = aRow + 1;
    col = aCol + 1;
    parkedCars = new int[(row - 1) * (col - 1)];
    for (int i = 0; i < (row - 1) * (col - 1); i++) {
        parkedCars[i] = 0;
    }
    arr = new char *[row];
    for (int i = 0; i < row; i++) {
        arr[i] = new char[col];
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            arr[i][j] = 0;
        }
    }
    fillInitialLot();
}


ParkingLot::~ParkingLot() {
    for (int i = 0; i < row; i++) {
        delete[] arr[i];
    }
    delete[] arr;
    delete[] parkedCars;
}

ParkingLot &ParkingLot::operator=(const ParkingLot &right) {
    if (&right != this) {
        if ((row != right.row || col != right.col) && ID != right.ID) {
            for (int i = 0; i < row; i++) {
                delete[] arr[i];
            }
            delete[] arr;
            delete[] parkedCars;
            ID = right.ID;
            row = right.row;
            col = right.col;
            if (row > 0 && col > 0) {
                arr = new char *[row];
                for (int i = 0; i < row; i++) {
                    arr[i] = new char[col];
                }
                parkedCars = new int[(row - 1) * (col - 1)];
            } else {
                arr = NULL;
                parkedCars = NULL;
            }
        }
        for (int i = 0; i < right.getArea(); i++) {
            parkedCars[i] = right.parkedCars[i];
        }
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                arr[i][j] = right.arr[i][j];
            }
        }
    }
    return *this;
}

void ParkingLot::printParkingLot() {
    cout << "ID: " << ID << ", (" << row - 1 << "," << col - 1
         << "), empty slots: " << numberEmptyPlaces() << ", parked cars: ";
    if (numberEmptyPlaces() == (row - 1) * (col - 1)) {
        cout << "no cars yet";
    } else {
        int counter = -1;
        for (int i = 0; i < getArea(); i++) {
            if (parkedCars[i] != 0)
                counter++;
        }
        for (int i = 0; i < getArea(); i++) {
            cout << parkedCars[i];
            if (counter > 0) {
                cout << ", ";
                counter--;
            } else
                break;
        }
    }
    cout << endl << endl;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (!(j > 9 && i == 0)) {
                if (j < 10)
                    cout << arr[i][j] << " ";
                else
                    cout << arr[i][j] << "  ";
            } else
                cout << j << " ";
        }
        cout << endl;
    }
}

void ParkingLot::fillInitialLot() const {
    arr[0][0] = ' ';
    for (int i = 1; i < col; i++) {
        arr[0][i] = static_cast<char>(i + 48);
    }
    for (int k = 1; k < row; k++) {
        arr[k][0] = static_cast<char>(k + 64);
    }
    for (int i = 1; i < row; i++) {
        for (int j = 1; j < col; j++) {
            arr[i][j] = '+';
        }
    }
}

void ParkingLot::coutLotInfo() const {
    cout << "ID: " << ID << ", Dim: (" << (row - 1) << "," << (col - 1) <<
         "), number of empty parking spaces: " << numberEmptyPlaces() << endl;
}

void ParkingLot::park_car(const int car) const {
    for (int i = 0; i < getArea(); i++) {
        if (parkedCars[i] == 0) {
            parkedCars[i] = car;
            break;
        }
    }
}

void ParkingLot::remove_car(const int car) const {
    for (int i = 0; i < getArea(); i++) {
        if (parkedCars[i] == car) {
            parkedCars[i] = 0;
            break;
        }
    }
}

bool ParkingLot::isOccupied(const string &carType, const int iRow, const int iCol) const {
    if (carType == "a") {
        return arr[iRow][iCol] != '+';
    } else if (carType == "b") {
        return arr[iRow][iCol] != '+' && arr[iRow - 1][iCol] != '+';
    } else
        return 1;
}

int ParkingLot::numberEmptyPlaces() const {
    int numberEmptyPlaces = 0;
    for (int i = 1; i < row; i++) {
        for (int j = 1; j < col; j++) {
            if (arr[i][j] == '+')
                numberEmptyPlaces++;
        }
    }
    return numberEmptyPlaces;
}

int ParkingLot::getId() const {
    return ID;
}

int ParkingLot::getArea() const {
    return ((row - 1) * (col - 1));
}

char **ParkingLot::getArr() const {
    return arr;
}

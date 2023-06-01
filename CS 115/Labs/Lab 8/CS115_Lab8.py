# -*- coding: utf-8 -*-


class Passenger:
    __AIRPORT_TAX: float = 0.03
    __FARE_LIMIT: int = 1000

    def __init__(self, name: str, surname: str, seat_no: str, fare: float):
        self.__passenger_name: str = name
        self.__passenger_surname: str = surname
        self.set_seat_number(seat_no)
        self.__fare: float = fare

    @property
    def name(self) -> str:
        return self.__passenger_name

    @property
    def surname(self) -> str:
        return self.__passenger_surname

    @property
    def seat_no(self) -> str:
        return self.__seat_number

    @property
    def fare(self) -> float:
        return self.__fare

    def set_seat_number(self, seat_number: str):
        if len(seat_number) == 3 and seat_number[0:2].isdigit() and seat_number[2].isalpha():
            self.__seat_number = seat_number[0:2] + seat_number[2].upper()
        else:
            self.__seat_number = 'unassigned'

    def calculate_fare(self):
        if self.__fare < self.__FARE_LIMIT:
            return self.__fare * (1 + self.__AIRPORT_TAX)
        return self.__fare

    def __lt__(self, other):
        if self.__passenger_surname == other.__passenger_surname:
            return self.__seat_number < other.__seat_number
        return self.__passenger_surname < other.__passenger_surname

    def __repr__(self):
        return f'{self.__passenger_surname}, {self.__passenger_name[0]}. ' \
               f'({self.__seat_number}) Fare: {self.calculate_fare():.2f}TL\n'


def load_passengers(filename: str) -> [Passenger]:
    passengers: [Passenger] = []
    with open(filename, 'r', encoding='utf-8') as passengers_file:
        for line in passengers_file:
            name, surname, fare, seat_no = line.strip('\n').split(';')
            passenger: Passenger = Passenger(name=name, surname=surname, seat_no=seat_no, fare=float(fare))

            if passenger not in passengers:
                passengers.append(passenger)
            else:
                print(f'duplicate - passenger {name} {surname} {seat_no} not added.')

    return passengers


def find_min_seat_index(passengers: [Passenger], start_index: int) -> int:
    min_index: int = start_index
    for i in range(start_index + 1, len(passengers)):
        if passengers[min_index].seat_no > passengers[i].seat_no:
            min_index = i
    return min_index


def sort_passengers_by_seat(passengers: [Passenger]) -> None:
    for i in range(len(passengers)):
        min_index: int = find_min_seat_index(passengers, i)
        passengers[i], passengers[min_index] = passengers[min_index], passengers[i]


def search_passenger_by_seat(passengers: [Passenger], search_seat: str) -> Passenger | None:
    if len(passengers) == 0:
        return None
    else:
        mid_index: int = len(passengers) // 2
        if passengers[mid_index].seat_no == search_seat:
            return passengers[mid_index]
        elif passengers[mid_index].seat_no > search_seat:
            return search_passenger_by_seat(passengers[:mid_index], search_seat)
        else:
            return search_passenger_by_seat(passengers[mid_index + 1:], search_seat)


def search_passenger_by_surname(passengers: [Passenger], surname: str) -> [Passenger]:
    if len(passengers) == 0:
        return []
    elif passengers[0].surname == surname:
        return [passengers[0]] + search_passenger_by_surname(passengers[1:], surname)
    else:
        return search_passenger_by_surname(passengers[1:], surname)


if __name__ == '__main__':
    passenger_list: [Passenger] = load_passengers('passengers.txt')
    print(f'Original list of Passengers:\n{passenger_list}')

    sort_passengers_by_seat(passenger_list)
    print(f'Passengers sorted by seat:\n{passenger_list}')

    seat_to_search: str = input('Enter seat to search: ')
    passenger_in_seat: Passenger = search_passenger_by_seat(passenger_list, seat_to_search)
    print(passenger_in_seat
          if passenger_in_seat else f'No passenger in {seat_to_search}')

    search_surname: str = input('Enter surname to search: ')
    matching_passengers: [Passenger] = search_passenger_by_surname(passenger_list, search_surname)
    print(f'List of matching passengers:\n{matching_passengers}'
          if matching_passengers else f'No passengers with surname {search_surname} found')

    passenger_list.sort()
    print(f'Passengers sorted by surname/seat number: {passenger_list}')

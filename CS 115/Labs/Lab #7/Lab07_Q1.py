# -*- coding: utf-8 -*-


class Passenger:
    # constant instances
    __FARE_LIMIT: int = 1000
    __FUEL_SURCHARGE: int = 150
    __AIRPORT_TAX: float = 0.03

    # constructor
    def __init__(self, name: str, surname: str, seat_no: str, fare: float) -> None:
        self.__name: str = name
        self.__surname: str = surname
        self.set_seat_no(seat_no)
        self.__fare: float = fare

    # getters
    @property
    def FARE_LIMIT(self) -> int:
        return self.__FARE_LIMIT

    @property
    def FUEL_SURCHARGE(self) -> int:
        return self.__FUEL_SURCHARGE

    @property
    def AIRPORT_TASK(self) -> float:
        return self.__AIRPORT_TAX

    @property
    def name(self) -> str:
        return self.__name

    @property
    def surname(self) -> str:
        return self.__surname

    @property
    def seat_no(self) -> str:
        return self.__seat_no

    @property
    def fare(self) -> float:
        return self.__fare

    # setters
    def set_seat_no(self, seat_no: str) -> None:
        if len(seat_no) != 3 or not (
                seat_no[0].isnumeric() and seat_no[1].isnumeric() and seat_no[2].isalpha()):
            self.__seat_no: str = 'unassigned'
            return

        row_number: int = int(seat_no[0:2])
        if isinstance(self, FirstClassPassenger):
            self.__seat_no: str = seat_no.upper() if 1 <= row_number <= 10 else 'priority assignment'
        elif isinstance(self, EconomyPassenger):
            self.__seat_no: str = seat_no.upper() if row_number > 10 else 'unassigned'
        else:
            self.__seat_no: str = seat_no

    # other methods
    def calculate_fare(self) -> float:
        return self.fare + self.FUEL_SURCHARGE + self.AIRPORT_TASK

    # magic methods
    def __eq__(self, other) -> bool:
        return self.seat_no == other.seat_no \
            and self.surname == other.surname

    def __lt__(self, other) -> bool:
        return self.seat_no < other.seat_no

    def __repr__(self) -> str:
        return f'{self.surname}, {self.name[0]}. ' \
               f'({self.seat_no}) Fare: {self.calculate_fare():.2f}TL\n'


class FirstClassPassenger(Passenger):
    # constructor
    def __init__(self, name: str, surname: str, seat_no: str, fare: float, meal_preference: str) -> None:
        super().__init__(name, surname, seat_no, fare)
        self.__meal_preference: str = meal_preference

    # getters
    @property
    def meal_preference(self) -> str:
        return self.__meal_preference

    # setters
    def set_meal_preference(self, meal_preference: str) -> None:
        self.__meal_preference: str = meal_preference

    # other methods
    def calculate_fare(self) -> float:
        fare: float = self.fare
        if fare > self.FARE_LIMIT:
            return fare + fare * self.AIRPORT_TASK
        return fare

    # magic methods
    def __repr__(self) -> str:
        return f'{super().__repr__()}Meal Preference: {self.meal_preference}\n'


class EconomyPassenger(Passenger):
    # constructor
    def __init__(self, name: str, surname: str, seat_no: str, fare: float) -> None:
        super().__init__(name, surname, seat_no, fare)

    # other methods
    def calculate_fare(self) -> float:
        return self.fare + self.FUEL_SURCHARGE

    # magic methods
    def __repr__(self) -> str:
        return super().__repr__()


# main
if __name__ == '__main__':
    # Initial part
    passengers: [Passenger] = []

    # Part a.
    with open('passengers.txt', 'r', encoding='utf-8') as passengers_file:
        for line in passengers_file:
            line: [str] = line.strip().split(';')

            # First Class Passenger
            if len(line) == 5:
                new_passenger: FirstClassPassenger = FirstClassPassenger(
                    name=line[0], surname=line[1], fare=float(line[2]), seat_no=line[3], meal_preference=line[4])

            # Economy Class Passenger
            elif len(line) == 4:
                new_passenger: EconomyPassenger = EconomyPassenger(
                    name=line[0], surname=line[1], fare=float(line[2]), seat_no=line[3])

            if new_passenger not in passengers:
                passengers.append(new_passenger)

            else:
                print(f'duplicate - passenger {new_passenger.name} '
                      f'{new_passenger.surname} {new_passenger.seat_no} not added...')
                del new_passenger

    # Part b.
    print(f'Original list of Passengers:\n{passengers}')

    # Part c.
    average_fare: float = sum([passenger.calculate_fare() for passenger in passengers]) / len(passengers)
    print(f'Average calculated fare: {average_fare:.2f}TL')

    # Part d.
    passengers.sort()

    # Part e.
    print('Assigned Seat Numbers: ', end='')
    for seat in [passenger.seat_no for passenger in passengers]:
        print(seat, end=' ') if seat not in ('unassigned', 'priority assignment') else None

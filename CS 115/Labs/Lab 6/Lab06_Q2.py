class Faculty:
    def __init__(self, name: str, office_building: str, room_no: int) -> None:
        self.__name = name
        self.__office_building = office_building
        self.__room_no = room_no

    def get_name(self) -> str:
        return self.__name

    def set_office_building(self, new_office_building: str) -> None:
        self.__office_building = new_office_building

    def set_room_no(self, new_room_no: int) -> None:
        self.__room_no = new_room_no

    def is_member(self, building_type: str) -> bool:
        return self.__office_building == building_type

    def __str__(self) -> str:
        return f'Faculty Name:\t{self.__name} ' \
               f'\t Address:\t{self.__office_building + str(self.__room_no)}'

    def __repr__(self) -> str:
        return f'(Faculty)\t{self.__name}' \
               f'\t{self.__office_building + str(self.__room_no)}'

    def __lt__(self, other) -> bool:
        if self.__office_building < other.__office_building:
            return True
        elif self.__office_building == other.__office_building:
            return self.__room_no < other.__room_no
        else:
            return False


def main() -> None:
    faculties: list = []

    with open('faculties.txt', 'r') as file:
        for line in file:
            name, office_building, room_no = line.split(',')
            faculties.append(Faculty(name, office_building.upper(), int(room_no)))

    building_name: str = input("Enter building name ('A','B','C'): ")

    for faculty in faculties:
        print(str(faculty)) if faculty.is_member(building_name.upper()) else ...

    faculty_name: str = input('\nEnter name: ')

    for faculty in faculties:
        if faculty_name == faculty.get_name():
            new_office_building, new_room_no = input('\nEnter new building: '), input('\nEnter room no: ')
            faculty.set_office_building(new_office_building.upper())
            faculty.set_room_no(int(new_room_no))
            break

    else:
        office_building, room_no = input('\nEnter building: '), input('\nEnter room no: ')
        faculties.append(Faculty(faculty_name, office_building.upper(), int(room_no)))

    faculties.sort()
    print(faculties)


if __name__ == '__main__':
    main()

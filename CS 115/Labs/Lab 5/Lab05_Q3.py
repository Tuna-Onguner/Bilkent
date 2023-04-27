def import_cars(file_name, cars_dict):
    with open(file_name, 'r') as cars:
        for plate in cars:
            brand, plate_number = plate.strip('\n').split(':')
            if brand in cars_dict:
                cars_dict[brand] += (plate_number,)
            else:
                cars_dict[brand] = (plate_number,)


def find_by_city(city_code, cars_dict):
    brands = set()
    total_cars = 0
    count_cars = 0
    for brand in cars_dict:
        for plate_number in cars_dict[brand]:
            total_cars += 1
            if plate_number[:2] == city_code:
                count_cars += 1
                brands.add(brand)
    return list(brands), count_cars / total_cars * 100


file_name = 'cars.txt'
cars_dict = dict()
import_cars(file_name, cars_dict)

city_code = input('Enter city code to search: ')
lcwgcc, pcwgcc = find_by_city(city_code, cars_dict)
print(f'Models with city code {city_code}: {lcwgcc}\nPercentage of cars from {city_code}: {pcwgcc}')

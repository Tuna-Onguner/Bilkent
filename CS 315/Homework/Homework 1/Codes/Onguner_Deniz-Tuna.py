#
# Bilkent University
# Fall 2022 - 2023
# CS315 Programing Languages
# Homework 1: Associative Arrays
# "Python Dictionaries"
#
# 18.11.2022, Friday
#
# Section 2
# Deniz Tuna Onguner
# ID: 22001788
#
# References:
# https://www.w3schools.com/python/python_dictionaries.asp
# https://www.geeksforgeeks.org/python-dictionary/
#


print("CS315 Homework 1\nPython Dictionaries\n")

# Initialize an associative array (Dictionary)
Age = {
    "Galadriel": 8372,
    "Elrond": 6518,
    "Arwen": 2901
}

# Initialize an empty dictionary
Rings = {}

# Type function
print(f"Type of dictionary: {type(Rings)}\n")

# Print a dictionary
print(f"Dictionary Age: {Age}\n"
      f"Dictionary Rings: {Rings}")

# Item count of a dictionary
item_count_Age = len(Age)
item_count_Rings = len(Rings)

print(f"\nItem count of Age is {item_count_Age}\n"
      f"Item count of Rings is {item_count_Rings}")

# Access an item from a dictionary
Age_Galadriel = Age["Galadriel"]

# Both Single-quotes and double-quotes are possible
Age_Elrond = Age['Elrond']

# get function is also available by default
Age_Arwen = Age.get("Arwen")

print(f"\nAge of Galadriel is {Age_Galadriel}\nAge of "
      f"Elrond is {Age_Elrond}\nAge of Arwen is {Age_Arwen}\n")

# Adding a new item to a dictionary
Age['Legolas'] = 2931
Rings['Elves'] = 3
Rings['Sauron'] = 0

# Increase an item's value
Rings['Sauron'] += 1

# Different type of keys are allowed
Rings[1] = "The one ring"

print(f"Dictionary Rings: {Rings}\n")

# Adding a new item with a different type of value
Age['Gandalf'] = 'Elder than time'

print("New items added\n"
      f"Dictionary Age: {Age}\n"
      f"Dictionary Rings: {Rings}\n")


# Search for a key and a value
def find_in_keys(key, dictionary):
    for k in dictionary.keys():
        if key == k:
            return True
    return False


print("Search for a key")
Key = "Gandalf"
print("Key exits\n") if find_in_keys(Key, Age) \
    else print("Key does not exist\n")


def find_in_values(val, dictionary):
    for v in dictionary.values():
        if val == v:
            return True
    return False


print("Search for a value")
Val = 20
print("Value exits\n") if find_in_values(Val, Age) \
    else print("Values does not exist\n")

# Delete an item from a dictionary
del Age['Gandalf']

# pop function is also available
Age.pop("Arwen")

print("Some items removed\n"
      f"Dictionary Age: {Age}\n")

# Copy a dictionary: copy method
Age_Copy = Age.copy()

# Empty a dictionary
Age = {}

# clear method is also possible
Rings.clear()

print("Dictionaries are cleaned\n"
      f"Dictionary Age: {Age}\n"
      f"Dictionary Rings: {Rings}\n")

Age = Age_Copy
print("Set Age to its copy\n")

# Access all values: values method
Sum = 0
for i in Age.values():
    Sum += i

print(f"Sum of all ages in Age dictionary is {Sum}\n")


# Prints the key-value pair
def foo(dictionary):
    print("Printing Key-value pairs:")
    for key in dictionary.keys():
        print(f"{key}: {dictionary[key]}")


foo(Age)

# Nested Dictionaries
# Initialize a nested dictionary
Age.clear()
Age['Elves'] = {"Galadriel": 8372, "Elrond": 6518, "Arwen": 2901}
Age['Dwarfs'] = {}
Age["Humans"] = {}
Age['Hobbits'] = {}

# Add items to nested dictionary
Age['Dwarfs']['Gimli'] = 139
Age['Humans']['Aragon'] = 87

print(f"\nNew Age dictionary as a nested one:\nNested dictionary Age {Age}")

# Remove an item from a nested dictionary
del Age['Elves']['Elrond']
Age['Elves'].pop("Arwen")

print(f"\nSome items removed:\nNested dictionary Age {Age}")

# Remove an inner dictionary from a dictionary
del Age['Hobbits']
Age.pop("Humans")

print(f"\nSome items removed:\nNested dictionary Age {Age}")

# Delete a dictionary
del Age
del Rings

print("\nAge is deleted\nRings is deleted\n")

# End of file

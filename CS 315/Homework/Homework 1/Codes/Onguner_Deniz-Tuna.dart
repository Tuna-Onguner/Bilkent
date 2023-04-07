/*
 * Bilkent University
 * Fall 2022 - 2023
 * CS315 Programing Languages
 * Homework 1: Associative Arrays
 * "Maps of Dart"
 *
 * 18.11.2022, Friday
 *
 * Section 2
 * Deniz Tuna Onguner
 * ID: 22001788
 *
 * References:
 * https://www.tutorialspoint.com/dart_programming/dart_programming_map.htm
 */


void main() {
  print("CS315 Homework 1\nMaps of Dart\n");

  // Initialize a map
  Map capitalsLatitudes = {
    "Paris": 48.864716,
    "Cairo": 30.033333,
    "Gitega": -3.42640,
    "Ankara": 39.925533,
    "Cape Town": -33.55
  };

  // An empty map
  Map countOfMoons = {};

  // Print the map
  print("Map: Latitudes of capitals $capitalsLatitudes");

  // Getting the element count
  var size = capitalsLatitudes.length;
  print("Element count of the map = $size");

  // isEmpty property
  var isEmpty = countOfMoons.isEmpty;
  print("\nThe map count of moons is ${isEmpty ? "empty" : "not empty"}\n");

  // Accesing elemets
  print("Latitude of Ankara is ${capitalsLatitudes["Ankara"]}");
  print("Latitude of Cairo is ${capitalsLatitudes["Cairo"]}\n");

  // Adding new elements
  countOfMoons["Earth"] = 1;
  countOfMoons["Jupiter"] = 80;

  print("After added new elements: $countOfMoons");
  
  // Modifiy an element
  countOfMoons["Jupiter"] -= 2;
  
  // Remove an element
  capitalsLatitudes.remove("Cape Town");
  
  // Updated maps
  print("\nLatitudes of capitals: $capitalsLatitudes\nCount of moons: $countOfMoons");
  
  // Search for keys
  print("\nSearch for keys");
  var key = "Paris";
  if (searchInKeys(capitalsLatitudes, key)) {
    print("Key exists");
  } else {
    print("Key does not exit");
  }
  
  // Search for values
  print("\nSearch for values");
  var val = 10;
  if (searchInValues(countOfMoons, val)) {
    print("Value exists");
  } else {
    print("Value does not exit");
  }
  
  // Key-value pairs
  print("\nCapitals Latitudes - Each key-value pair one by one:");
  foo(capitalsLatitudes);
  
  // Clear a map
  capitalsLatitudes.clear();
  countOfMoons = {};
  
  print('\nMaps are cleared:\nLatitudes of capitals: '
    '$capitalsLatitudes\nCount of moons: $countOfMoons');
  
  // Nested maps
  capitalsLatitudes = {
    "Europa": {"Paris": 48.864716},
    "Africa": {"Gitega": -3.42640, "Cairo": 30.033333},
    "Asia": {"Ankara": 39.925533}
  };
  
  // Accessing a nested map
  print("\nNested maps");
  print("Latitude of Paris is ${capitalsLatitudes["Europa"]["Paris"]}");
  
  // Modify an element of a nested map
  capitalsLatitudes["Africa"]["Gigeta"] = -3.0;
  
  // Adding a new element
  capitalsLatitudes["Europa"]["Rome"] = 41.902782;
  
  print("\nUpdated Latitudes of capitals: $capitalsLatitudes\n");
  
  // Delete an elemet from a nested map
  capitalsLatitudes.remove("Asia");
  
  print("Asia is removed from Latitudes of capitals: $capitalsLatitudes\n");
  
  // Adding a new map to a nested map
  capitalsLatitudes["South America"] = {"Santiago": -33.447487, "Buenos Aires": -34.603722};
  print("Final map of Latitudes of capitals: $capitalsLatitudes\n");
  
  capitalsLatitudes.clear();
}

bool searchInKeys(Map map, var key) {
  for (var k in map.keys) {
    if (k == key) {
      return true;
    }
  }
  return false;
}

bool searchInValues(Map map, var val) {
    for (var v in map.values) {
    if (v == val) {
      return true;
    }
  }
  return false;
}

void foo(Map map) {
  map.forEach((key, val) => print("$key: $val"));
}

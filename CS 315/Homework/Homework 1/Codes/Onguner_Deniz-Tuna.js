/*
 * Bilkent University
 * Fall 2022 - 2023
 * CS315 Programing Languages
 * Homework 1: Associative Arrays
 * "JavaScript Associative Arrays"
 *
 * 18.11.2022, Friday
 *
 * Section 2
 * Deniz Tuna Onguner
 * ID: 22001788
 *
 * References:
 * https://www.geeksforgeeks.org/how-to-create-dictionary-and-add-key-value-pairs-dynamically/
 */


console.log("CS315 Homework 1\nJavaScript Associative Arrays\n")

// Initialize an associative array
let grades = {
    "Hasan": 95,
    "Deniz": 50,
    "Alper": 10,
    "Kerem": 40
}

// Initialize an empty associative array
let letterGrades = {}

// Print the associative array
console.log("Grades =", grades)
console.log("Letter Grades =", letterGrades)

// Getting element count of an associative array
let size = Object.keys(grades).length
console.log("\nElement count of Grades: " + size)
let emptySize = Object.keys(letterGrades).length
console.log("Element count of Letter Grades: " + emptySize)

// Accessing elements by keys
let average = (grades["Hasan"] + grades["Deniz"] + grades["Alper"] + grades["Kerem"]) / size;
console.log("\nAverage of Grades: " + average)
console.log("grade of Hasan =", grades["Hasan"])

// Additionally "associative_array.<key>" is also available
console.log("grade of Alper =", grades.Alper)

// Adding new items
letterGrades[99] = "A+"
letterGrades[90] = "A"
letterGrades[80] = "A-"

grades["Emma"] = 65
grades["Jack"] = 75

console.log("\nUpdated Grades =", grades)
console.log("Updated Letter Grades =", letterGrades)

// Change a value
grades["Emma"] += 20
console.log("\nEmma's grade is updated to", grades.Emma)

// Delete an element
delete grades.Jack
delete letterGrades[99]
console.log("\nUpdated Grades =", grades)
console.log("Updated Letter Grades =", letterGrades)

// Modify an element
grades["Kerem"] = 90

// Search for keys
function searchKeys(key, aa) {
    for (let k in aa)
        if (k === key)
            return true
    return false
}

console.log("\nSearch for a key")
let key = "Emma"
if (searchKeys(key, grades))
    console.log("Key exists")
else
    console.log("Key does not exist")

// Search for values
function searchValues(val, aa) {
    for (let v in aa)
        if (aa[v] === val)
            return true
    return false
}

console.log("\nSearch for a value")
let val = 85
if (searchValues(val, grades))
    console.log("Value exists")
else
    console.log("Value does not exist")

delete grades.Emma

// Prints the key-value pairs
console.log("\nfoo function: Prints the key-value pairs")
function foo(aa) {
    for (let key in aa)
        console.log(key, "=", aa[key])
}

foo(grades)

// Clear an associative array
for (let item in letterGrades)
    delete letterGrades[item]

console.log("\nCleared Letter Grades =", letterGrades)

// Nested associative arrays
// Initialize a nested associative arrays
console.log("\nNested associative arrays")
letterGrades = {
    "MATH102": {10: "F", 30: "C", 50: "A"},
    "CS315": {20: "F", 60: "See the curve", 90: "A", 100: "A+"}
}

// Accessing elements
let gradeMATH102 = letterGrades["MATH102"][10]
let gradeCS315 = letterGrades["CS315"][90]
console.log("Grade for MATH101:", gradeMATH102, "\n" +
    "Grade for CS315:", gradeCS315)

// Adding new elements
letterGrades["MATH102"][40] = "B"

// Deleting Elements
delete letterGrades["CS315"][100]

// Print a nested associative array
console.log("\nLetter Grades Updated =", letterGrades)

// Delete inner associative array from another associative array
delete letterGrades["MATH102"]

console.log("\nLetter Grades Updated, MATH102 is removed =", letterGrades)

// End of file

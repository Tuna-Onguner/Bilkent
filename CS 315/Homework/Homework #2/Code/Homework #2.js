/*
 * Bilkent University
 * Fall 2022 - 2023
 *
 * CS315 Programing Languages
 * Homework 2: User-Located Loop Control Mechanisms
 * Language: JavaScript, version: ES6+
 *
 * 03.12.2022, Saturday
 *
 * Section 2
 * Deniz Tuna Onguner
 * ID: 22001788
 *
 * References:
 * https://www.tutorialspoint.com/javascript/javascript_loop_control.htm
 */

console.log("CS315 Homework 2\nUser-Located Loop Control Mechanisms in JavaScript\n")

// In JavaScript, User-Located Loop Control Mechanisms are
// unconditional, either unlabeled or labeled

let temp = 0;

// Unlabeled Single Loop Examples
// Ex. 1
while (true) {
    temp += 1
    if (temp === 5)
        break
}

// The expected output: 5
console.log("Example 1 output:", temp)

// Reset temp
temp = 0

// Ex. 2
while (temp < 10) {
    temp += 1
    if (temp < 5)
        continue
    break
}

// The expected output: 5
console.log("Example 2 output:", temp)

// Reset temp
temp = 0

// Unlabeled Nested Loop Examples
// Ex. 3
for (let i = 0; i < 10; i++) {
    if (i >= 5)
        continue
    for (let j = 0; j < 20; j++) {
        temp += j
        if (j >= 5)
            break
    }
}

// The expected output: 85
console.log("Example 3 output:", temp)

// Reset temp
temp = 0

// Labeled Nested Loop Examples
// Ex. 4
outerLoop:
for (let i = 0; i < 10; i++) {
    if (i >= 5)
        continue
    // innerLoop:
    for (let j = 0; j < 20; j++) {
        temp += j
        if (j >= 5)
            break outerLoop
    }
    temp += i
}

// The expected output: 15
console.log("Example 4 output:", temp)

// Reset temp
temp = 0

// Ex. 5
outerLoop:
for (let i = 0; i < 10; i++) {
    if (i >= 5)
        break
    // innerLoop:
    for (let j = 0; j < 20; j++) {
        temp += j
        if (j >= 5)
            continue outerLoop
    }
    temp += i
}

// The expected output: 75
console.log("Example 5 output:", temp)

// End of the file

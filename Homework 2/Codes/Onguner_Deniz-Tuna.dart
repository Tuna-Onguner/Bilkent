/*
 * Bilkent University
 * Fall 2022 - 2023
 *
 * CS315 Programing Languages
 * Homework 2: User-Located Loop Control Mechanisms
 * Language: Dart, version: 2.18.5
 *
 * 03.12.2022, Saturday
 *
 * Section 2
 * Deniz Tuna Onguner
 * ID: 22001788
 *
 * References:
 * https://www.tutorialspoint.com/dart_programming/dart_programming_loops.htm
 * https://www.geeksforgeeks.org/dart-loop-control-statements-break-and-continue/
 * https://www.geeksforgeeks.org/labels-in-dart/
 */

void main() {
    print("CS315 Homework 2\nUser-Located Loop Control Mechanisms in Dart\n");

    var ex = 0;

    // Example 1
    while (true) {
        ex = ex + 1;
        if (ex >= 5)
            break;
    }

    // Expected output: 5
    print("Example 1:   $ex");

    ex = 0;

    // Example 2
    while (ex < 10) {
        while (true) {
            ex += 1;
            if (ex == 10)
                break;
        }
    }

    // Expected output: 10
    print("Example 2:   $ex");

    ex = 0;

    // Example 3
    outerLoop: for (var i = 0; i < 10; i++) {
        innerLoop: for (var j = 0; j < 5; j++) {
            ex += j;
            if (i == 5)
                break outerLoop;
        }
        ex += i;
    }
    
    // Expected output: 60
    print("Example 3:   $ex");

    ex = 0;

    // Example 4
    while (ex < 10) {
        while (ex < 20) {
            ex += 1;
            if (ex % 2 == 0)
                continue;
            ex += 10;
        }
    }
    
    // Expected output: 23
    print("Example 4:   $ex");

    ex = 0;

    // Example 5
    outerLoop: for (var i = 0; i < 10; i++) {
        innerLoop: for (var j = 0; j < 5; j++) {
            ex += j;
            if (i == 5)
                continue outerLoop;
        }
        ex += i;
    }
    
    // Expected output: 130
    print("Example 5:   $ex");

    ex = 0;
    print("\n");

}

// End of the file

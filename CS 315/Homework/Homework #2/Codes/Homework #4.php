<?php

/*
 * Bilkent University
 * Fall 2022 - 2023
 *
 * CS315 Programing Languages
 * Homework 2: User-Located Loop Control Mechanisms
 * Language: PHP, version: 8.1.13
 *
 * 03.12.2022, Saturday
 *
 * Section 2
 * Deniz Tuna Onguner
 * ID: 22001788
 *
 * References:
 * https://www.w3schools.com/php/php_looping_break.asp
 * https://www.php.net/manual/en/control-structures.break.php
 * https://www.php.net/manual/en/control-structures.continue.php
 */

echo "CS315 Homework 2\nUser-Located Loop Control Mechanisms in PHP\n";

$var = 0;

// Example 1
for (;;) {
    $var = $var + 1;
    if ($var >= 7)
        break;
}

echo "\nOutput[ Example 1 ]:  $var";

$var = 0;

// Example 2
for (; $var < 100; $var++) {
    if ($var % 2 != 0)
        continue;
    $var *= 10;
}

echo "\nOutput[ Example 2 ]:  $var";

$var = 0;

// Example 3
while (true) {
    $var++;
    if ($var >= 7)
        break;
}

echo "\nOutput[ Example 3 ]:  $var";

$var = 0;

// Example 4
while (true) {
    $var++;
    if ($var == 13)
        goto exitWhileLoop;
} exitWhileLoop:

echo "\nOutput[ Example 4 ]:  $var";

$var = 0;

// Example 5
echo "\n\nExample 5:\n";
for (;;) {
    echo "continue is ran\n";

    while (true) {
        $var++;
        if ($var == 5)
            continue 2;
        else if ($var == 9)
            break 2;
    }
}

echo "Output[ Example 5 ]:  $var";

$var = 0;

// End of the file
?>

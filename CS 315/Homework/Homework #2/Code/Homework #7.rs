/*
 * Bilkent University
 * Fall 2022 - 2023
 *
 * CS315 Programing Languages
 * Homework 2: User-Located Loop Control Mechanisms
 * Language: Rust, version: 1.65.0
 *
 * 03.12.2022, Saturday
 *
 * Section 2
 * Deniz Tuna Onguner
 * ID: 22001788
 *
 * References:
 * https://doc.rust-lang.org/book/ch03-05-control-flow.html
 * https://doc.rust-lang.org/reference/expressions/loop-expr.html
 */

fn main() {
    println!("CS315 Homework 2\nUser-Located Loop Control Mechanisms in Rust\n");

    let mut var = 0;
    let set = &[1, 2, 3, 4, 5];

    // Example 1
    while var < 10 {
        var = var + 1;
        if var % 5 == 0 && var != 0 {
            break;
        }
    }

    println!("Example 1 output =  {}", var);

    var = 0;

    // Example 2
    for i in set {
        if i % 2 == 0 {
            continue;
        }
        var += i;
    }

    println!("Example 2 output =  {}", var);

    var = 0;

    // Example 3
    'loopEx3: loop {
        var += 1;
        if var == 7 {
            break 'loopEx3;
        }
    }

    println!("Example 3 output =  {}", var);

    var = 0;

    // Examle 4
    'outerLoop: loop {
        var = var + 1;
        loop {
            var = var * 10;
            if var > 200 {
                break 'outerLoop;
            } else if var % 10 == 0 {
                continue 'outerLoop;
            }
            break;
        }
    }

    println!("Example 4 output =  {}", var);

    let (mut temp1, mut temp2) = (0, 0);

    // Example 5
    let calc = loop {
        if temp1 > 20 {
            break temp2;
        } else {
            temp1 += 1;
            temp2 = temp1 * 2;
        }
    };

    var = calc;

    println!("Example 5 output =  {}", var);
}

// End of the file

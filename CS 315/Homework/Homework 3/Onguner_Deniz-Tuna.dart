/*
 * Bilkent University
 * Fall 2022 - 2023
 *
 * CS315 Programing Languages
 * Homework 3: Subprograms in Dart
 * 
 * Language: Dart, version: 2.18.15
 * Typed on VSCode (https://code.visualstudio.com)
 *
 * Section 2
 * Deniz Tuna Onguner
 * Bilkent ID: 22001788
 * 
 * December 17, 2022 - Saturday
 *
 * References:
 * https://dart.dev
 * https://o7planning.org/14061/dart-closures
 */

// Required libraries
import 'dart:math';

// Global constants
const double t = 10;
const double pi = 3.14;
const localTest = "Global";

// main
void main(List<String> arguments) {
  print("CS315 Homework 3\nSubprograms in Dart");

/*  1. Nested subprogram definitions  */
  print("\nOutput[ 1. Nested subprogram definitions ]:");

  double distance = 100;
  // !ERROR: var a = getAcceleration(10);

  double getAcceleration(final double time) {
    // !ERROR: var v = getVelocity();

    double getVelocity() {
      return distance / time; // distance: local variable in main
    }

    return getVelocity() / t; // t: global constant
  }

  var acceleration = getAcceleration(10);
  // !ERROR: var v = getVelocity();

  var area = getAreaCircle(10); // getAreaCircle: global function
  // !ERROR: var s = square(10);

  print("\tAcceleration is ${acceleration} m/s^2");
  print("\tArea is ${area} m^2");

/*  2. Scope of local variables  */
  print("\nOutput[ 2. Scope of local variables ]:");

  // Dart is a lexically scoped language
  var localTest = "inMain";

  {
    // First layer scope
    var localTest = "inScope";
    print("\t$localTest");

    {
      // Second layer scope
      print("\t$localTest");

      {
        // Third layer scope
        var localTest = "inInnerScope";
        print("\t$localTest");
      }
    }
  }

  print("\t$localTest");

/*  3. Parameter passing methods  */
  print("\nOutput[ 3. Parameter passing methods ]:");

  // Simple parameters
  var myVar = 20;
  // Dart is a pass by value language
  increment(myVar); // increment: global function

  print("\tmyVar is ${myVar}"); // Expected output: 20

  // Named parameters
  // printMessage: global function
  // !ERROR: printMessage();
  // !ERROR: printMessage(message: "Hello, Dart");
  printMessage(count: 3); // output: null [3]
  printMessage(message: "Hello, World", count: 2); // output: Hello, World [2]

  // Optional positional parameters
  bool? equals(String a, [String? b]) {
    if (b != null) return a == b;
    return null;
  }

  var a = "ABC";
  var b = "ABC";
  var c = "123";

  print("\ta equals b: ${equals(a, b)}"); // output: true
  print("\ta equals c: ${equals(a, c)}"); // output: false
  print("\ta equals null: ${equals(a)}"); // output: null

  // Passing other functions as parameters
  num m3(num a) {
    return a % 3;
  }

  print("\t9 is multiple of 3: ${isMultipleOf3(9, m3)}");

/*  4. Keyword and default parameters  */
  print("\nOutput[ 4. Keyword and default parameters ]:");

  // Named parameters with default values
  num? logX({num? value, num base = 10}) {
    if (value != null) return log(value) / log(base);
    return null;
  }

  final val = 10;

  print("\tLog10(10) equals 1: ${logX(value: val) == 1}"); // output: true
  print("\tLog2(10): ${logX(value: val, base: 2)}"); // output: ~3
  print("\tLog10(null): ${logX()}"); // output: null

  // Optional positional parameters with default values
  // foo: global function
  var fooOutput1 = foo();
  var fooOutput2 = foo(null);
  var fooOutput3 = foo(3);

  print("\tOutput[ foo() ]: $fooOutput1"); // output: true
  print("\tOutput[ foo(null) ]: $fooOutput2"); // output: false
  print("\tOutput[ foo(3) ]: $fooOutput3"); // output: false

/*  5. Closures  */
  print("\nOutput[ 5. Closures ]:");

  var result;
  var number = 7;

  // Direct called closure
  (num number) {
    result = number % 2;
  }(number);

  print("\tResult 7 (mod 2): $result");

  // Variable identifier closure
  var mod3 = (num number) {
    return number % 3;
  };

  print("\tResult 7 (mod 3): ${mod3(number)}");

  // Pass closures to functions as parameters
  var is7MultipleOf3 = isMultipleOf3(7, mod3);
  print("\t7 is multiple of 3: $is7MultipleOf3");
} // End main

// Global functions
double getAreaCircle(double radius) {
  double square(final double a) {
    return a * a;
  }

  return square(radius) * pi; // pi: global constant
}

void increment(var a) {
  a = a + 1;
}

void printMessage({String? message, required int? count}) {
  count = count ?? 0;
  for (var i = 0; i < count; i++) {
    print("\t$message");
  }
}

bool foo([num? number = 0]) {
  return number == 0;
}

bool isMultipleOf3(final int a, num Function(num n) f) {
  return f(a) == 0;
}

// End file

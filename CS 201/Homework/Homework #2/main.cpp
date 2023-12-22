//
// Deniz Tuna Onguner
// ID: 22001788
// CS 201 - Section 1
// Homework 2
// 06.04.2022, Wed
// Spring 2021/2022
// Bilkent University
//

#include <ctime>
#include <iostream>

long long func_1(long, long, long);

long long func_2(long, long, long);

long long func_3(long, long, long);

const static long nList[] = {100000000,
                             200000000,
                             300000000,
                             400000000,
                             500000000,
                             600000000,
                             700000000,
                             800000000,
                             900000000,
                             1000000000};

const static long pList[] = {101, 1009, 10007};

// main function
int main() {

    double duration;
    for (int i = 0; i < 10; i++) {
        clock_t start_time = clock();
        // functions input sequence: function(base, power, modulus) = function(a, n, p)
        // clock begin...

        for (int j = 0; j < 1000; j++)
            //func_1(255300, nList[i], pList[0]);
            //func_2(255300, nList[i], pList[1]);
            func_3(255300, nList[i], pList[2]);

        // ...clock end
        duration = 1000 * double(clock() - start_time) / CLOCKS_PER_SEC;
        std::cout << "Execution took " << duration / 1000 << " milliseconds." << std::endl;
    }

    return 0;
} // end main function

// Algorithms
/**
 * Algorithm 1 (Naive algorithm):
 * This function multiplies the a with itself in
 * a for loop and takes modulus p in each step.
 * After the loop ends, it returns the result.
 * @param a base
 * @param n power
 * @param p modulus
 * @return mod p of a^n
 */
long long func_1(const long a, const long n, const long p) {
    long long mod = 1;
    for (int i = 1; i <= n; i++)
        mod = a * mod % p;
    return mod;
}

/**
 * Algorithm 2 (Naive algorithm with Cycle shortcut):
 * This function has the same behavior as previous one until the step i which
 * satisfies a^i≡1 (mod p). After that step, it calculates a^(mod i) and returns
 * the result. Note that if n is less than i, the algorithm is equivalent to the previous one.
 * @param a base
 * @param n power
 * @param p modulus
 * @return mod p of a^n
 */
long long func_2(const long a, const long n, const long p) {
    long long mod = 1;
    for (int i = 1; i <= n; i++) {
        mod = a * mod % p;
        if (mod == 1) {
            for (int j = 1; j <= n % i; j++)
                mod = a * mod % p;
            return mod;
        }
    }
    return mod;
}

/**
 * Algorithm 3 (Recursive Algorithm):
 * This function is recursively calculates
 * a * (a^(n−1)/2 (mod p)) * (a^(n−1)/2 (mod p)) (mod p) if n is odd; and calculates
 * (a^(n/2) (mod p)) * (a^(n/2) (mod p)) (mod p) if n is even. Note that n=1 is the base case for the function.
 * @param a base
 * @param n power
 * @param p modulus
 * @return mod p of a^n
 */
long long func_3(const long a, const long n, const long p) {
    long long res;
    if (n <= 1) {
        return a % p;
    } else if (n % 2 == 0) {
        res = func_3(a, n / 2, p);
        return res * res % p;
    } else {
        res = func_3(a, (n - 1) / 2, p);
        return a * res * res % p;
    }
} // end algorithms

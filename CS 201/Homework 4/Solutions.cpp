//
// Deniz Tuna Onguner
// ID: 22001788
// CS201, Section 1
// Homework 4
// Bilkent University
// 14.05.2022, Sat
//

#include "Solutions.h"

int processPriority(const char operation);

bool isFantabulous(Stack<int> substack);

void fantabulous(string list, int length) {
    // Non-array solution
    int num, c = 0, dig;
    length = list.size();
    stringstream sstream;
    string str = string();
    Stack<int> mainstack, substack;
    for (int i = 0; i < length + 1; i++) {
        dig = list[i];
        if (dig == 44 || i == length) {
            sstream << str;
            sstream >> num;
            mainstack.push(num);
            str = string();
            sstream.clear();
        } else
            str += dig;
    }
    while (!mainstack.isEmpty()) {
        mainstack.pop(num);
        substack.push(num);
        if (isFantabulous(substack))
            c++;
        if (!mainstack.isEmpty())
            continue;
        else {
            while (!substack.isEmpty()) {
                substack.pop(num);
                mainstack.push(num);
            }
            mainstack.pop();
        }
    }
    cout << "The number of Fantabulous subsequences: " << c << endl;
    // Array-based solution
    /*
    stringstream sstream;
    string str = string();
    Stack<int> mainstack, reversedstack;
    int num, c = 0, arr[length - 1], index = 0;
    for (int i = 0; i < list.size() + 1; i++) {
        if (list[i] == 44 || i == list.size()) {
            sstream << str;
            sstream >> num;
            arr[index] = num;
            str = string();
            sstream.clear();
            index++;
        } else
            str += list[i];
    }
    for (int i = 0; i < length; i++) {
        for (int j = i; j < length; j++) {
            for (int k = i; k <= j; k++)
                reversedstack.push(arr[k]);
            while (!reversedstack.isEmpty()) {
                reversedstack.pop(num);
                mainstack.push(num);
            }
            if (isFantabulous(mainstack))
                c++;
        }
    }
    cout << "The number of Fantabulous subsequences: " << c << endl;
     */
}

void subset(string intervals, int length) {
    int num, c = 0;
    stringstream sstream;
    string str = string();
    length = intervals.length();
    Interval interval_1, interval_2;
    Stack<Interval> mainstack, secondarystack;
    for (int i = 0; i < length + 1; i++) {
        if (intervals[i] == 44 || i == length) {
            c++;
            sstream << str;
            sstream >> num;
            if (c % 2 == 0) {
                interval_1.end = num;
                if (!mainstack.isEmpty()) {
                    mainstack.pop(interval_2);
                    if (interval_2.end > interval_1.beg) {
                        interval_1.beg = interval_2.beg;
                        if (interval_2.end > interval_1.end)
                            interval_1.end = interval_2.end;
                    } else
                        mainstack.push(interval_2);
                }
                mainstack.push(interval_1);
            } else
                interval_1.beg = num;
            str = string();
            sstream.clear();
        } else
            str += intervals[i];
    }
    while (!mainstack.isEmpty()) {
        mainstack.pop(interval_2);
        secondarystack.push(interval_2);
    }
    cout << "Joined subset: ";
    while (!secondarystack.isEmpty()) {
        secondarystack.pop(interval_1);
        cout << "(" << interval_1.beg << ":" << interval_1.end << ")";
        if (!secondarystack.isEmpty())
            cout << ", ";
    }
    cout << endl;
}

string infix2prefix(string expression) {
    Stack<char> stack;
    string postfixexpression = string();
    const string constexpression = expression;
    expression = string(constexpression.rbegin(), constexpression.rend());
    char digit, top = 0xFF, pop = 0xFF;
    for (int i = 0; i < expression.length(); i++) {
        digit = expression[i];
        if ((digit >= 48 && digit <= 57))
            postfixexpression += digit;
        else if (digit == 41)
            stack.push(digit);
        else if (digit == 40) {
            while (pop != 41) {
                stack.pop(pop);
                postfixexpression += pop;
                stack.getTopItem(pop);
            }
            stack.pop();
        } else {
            if (stack.isEmpty())
                stack.push(digit);
            else {
                stack.getTopItem(top);
                if (processPriority(digit) >= processPriority(top))
                    stack.push(digit);
                else {
                    while (processPriority(top) > processPriority(digit)) {
                        stack.pop(top);
                        postfixexpression += top;
                        if (stack.isEmpty())
                            break;
                    }
                    stack.push(digit);
                }
            }
        }
    }
    while (!stack.isEmpty()) {
        stack.pop(top);
        postfixexpression += top;
    }
    return "Prefix form of " + constexpression + " is " +
           string(postfixexpression.rbegin(), postfixexpression.rend());
}

void evaluatePrefix(string expression) {
    int digit;
    Stack<double> stack;
    double num1, num2, finalresult;
    for (int i = expression.length() - 1; i >= 0; --i) {
        digit = expression[i];
        if (digit >= 48 && digit <= 57)
            stack.push(digit - 48);
        else {
            stack.pop(num1);
            stack.pop(num2);
            switch (digit) {
                case 42:
                    stack.push(num1 * num2);
                    break;
                case 43:
                    stack.push(num1 + num2);
                    break;
                case 45:
                    stack.push(num1 - num2);
                    break;
                case 47:
                    stack.push(num1 / num2);
                    break;
            }
        }
    }
    stack.pop(finalresult);
    cout << "Result of " << expression << " is: " << finalresult << endl;
}

int processPriority(const char operation) {
    switch (operation) {
        case 42:
        case 47:
            return 2;
        case 43:
        case 45:
            return 1;
        default:
            return 0;
    }
}

bool isFantabulous(Stack<int> substack) {
    if (substack.getItemCount() > 0) {
        int top,
            index = 0,
            largest = -1,
            secondLargest = -1,
            largestIndex = -1,
            secondLargestIndex = -1;
        while (!substack.isEmpty()) {
            substack.pop(top);
            if (top > largest) {
                secondLargest = largest;
                secondLargestIndex = largestIndex;
                largest = top;
                largestIndex = index;
            } else if (top > secondLargest && top < largest) {
                secondLargest = top;
                secondLargestIndex = index;
            }
            index++;
        }
        return (largestIndex > secondLargestIndex);
    } else
        return false;
}

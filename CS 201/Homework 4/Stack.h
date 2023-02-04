//
// Deniz Tuna Onguner
// ID: 22001788
// CS201, Section 1
// Homework 4
// Bilkent University
// 14.05.2022, Sat
//

#ifndef _STACK_H
#define _STACK_H
#define MAX_STACK 50

#include <iostream>

using namespace std;

struct Interval {
    int beg, end;
};

template<class StackItemType>
class Stack {
public:
    Stack();

    bool isEmpty() const;

    bool push(const StackItemType newItem);

    bool pop();

    bool pop(StackItemType &currentTop);

    bool getTopItem(StackItemType &currentTop);

    int getItemCount() const;

private:
    StackItemType items[MAX_STACK];
    int top;
};

#endif //_STACK_H

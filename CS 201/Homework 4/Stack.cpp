//
// Deniz Tuna Onguner
// ID: 22001788
// CS201, Section 1
// Homework 4
// Bilkent University
// 14.05.2022, Sat
//

#include "Stack.h"

template<class StackItemType>
Stack<StackItemType>::Stack() : top(-1) {

}

template<class StackItemType>
bool Stack<StackItemType>::isEmpty() const {
    return top < 0;
}

template<class StackItemType>
bool Stack<StackItemType>::push(const StackItemType newItem) {
    if (top >= MAX_STACK - 1)
        return false;
    else {
        top++;
        items[top] = newItem;
        return true;
    }
}

template<class StackItemType>
bool Stack<StackItemType>::pop() {
    if (isEmpty())
        return false;
    else {
        top--;
        return true;
    }
}

template<class StackItemType>
bool Stack<StackItemType>::pop(StackItemType &currentTop) {
    if (isEmpty())
        return false;
    else {
        currentTop = items[top];
        top--;
        return true;
    }
}

template<class StackItemType>
bool Stack<StackItemType>::getTopItem(StackItemType &currentTop) {
    if (isEmpty())
        return false;
    else {
        currentTop = items[top];
        return true;
    }
}

template<class StackItemType>
int Stack<StackItemType>::getItemCount() const {
    return top;
}

template
class Stack<int>;

template
class Stack<Interval>;

template
class Stack<char>;

template
class Stack<double>;

template
class Stack<string>;

template
class Stack<float>;

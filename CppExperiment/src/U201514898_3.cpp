/**
 * @file U201514898_3.cpp
 * @brief object-oriented stack implementation, opertor-overloaded version.
 *
 * @details
 * Again, this file implemented a int stack and some
 * overloaded operators to operate it, developed using
 * object oriented method. a useless main function is
 * added to test the correctness and robustness of the stack.
 *
 * @date 2017-11-07
 * @author Sixu Hu
 *
 * @copyright
 * Copyright 2017 Sixu Hu <husixu1@hotmail.com>
 * <br><br>
 * This file is part of C++ Experiment.
 * <br><br>
 * C++ Experiment free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * <br><br>
 * C++ Experiment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with C++ Experiment.  If not, see <http://www.gnu.org/licenses/>.
**/

#include<stdexcept>
#include<iostream>
#include<fstream>

/// @brief the stack class
class STACK {
private:
    /// @brief the array used to store stack elements
    int *const elems;
    /// @brief stack capacity
    const int max;
    /// @brief stack size
    int pos;

public:
    /// @brief constructor, initialize the stack with the size of m
    /// @param[in] m the size of the stack
    STACK(int m);

    /// @brief copy-constructor, initialize the stack with another stack
    /// @param[in] s the stack to copy
    STACK(const STACK &s);

    /// @brief get the capacity of the stack
    /// @return stack capacity
    virtual int size() const;

    /// @brief get the size of the stack
    /// @return stack size
    virtual operator int() const;

    /// @brief get the element with index x
    /// @param[in] x the element index to get
    /// @return the element with index x
    virtual int operator [](int x) const;

    /// @brief push an element to the stack
    /// @param[in] e the element to push
    /// @return the stack after pushing
    virtual STACK& operator <<(int e);

    /// @brief pop an element from the stack
    /// @param[out] e the poped element
    /// @return the stack after poping
    virtual STACK& operator >>(int &e);

    /// @brief copy a stack to this stack
    /// @param s the stack to copy
    /// @return the copied stack
    /// @note the current stack's element will be deleted
    virtual STACK& operator =(const STACK &s);

    /// @brief print the elements of the thread
    virtual void print() const;

    /// @brief de-constructor, destroy the stack
    virtual ~STACK();
};

namespace Utils {
/// @brief convert string to int
/// @param[in] s the string to convert
/// @param[out] out the output int
/// @return if the conversion is successful
bool convertToInt(const std::string &s, int &out);
}

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace Utils;

    // arguments
    string arg;
    int number;

    // stack for test
    STACK *stack = nullptr ;

    // file to write
    ofstream outFile;
    outFile.open("U201514898_3.TXT");

    auto coutBackup = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

#define CHECK_IF_LAST_ARG if (i == argc - 1){ \
    cerr << "paramter not found after: " << arg << endl; \
    return 1; \
}

    try {
        for(int i=1; i < argc; ++i) {
            arg = string(argv[i]);
            if (arg == "-S" || arg == "-s") {
                CHECK_IF_LAST_ARG;

                arg = string(argv[++i]);
                if (! convertToInt(arg, number)) {
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                stack = new STACK(number);
                cout << "S  " << number;
            } else if (arg == "-I" || arg == "-i") {
                cout << "I";

                for(++i; i < argc; ++i) {
                    arg = string(argv[i]);
                    // until there's noting to covert
                    if(!convertToInt(arg, number)) {
                        break;
                    }
                    (*stack) << number;
                }
                if(i != argc)
                    --i;

                if(int(*stack)) {
                    cout << "  ";
                    stack->print();
                }
            } else if (arg == "-O" || arg == "-o") {
                CHECK_IF_LAST_ARG

                cout << "O";
                arg = string(argv[++i]);
                if (! convertToInt(arg, number)) {
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                int garbage;
                for(int j=0; j<number; ++j) {
                    (*stack) >> garbage;
                }

                if(int(*stack)) {
                    cout << "  ";
                    stack->print();
                }

            } else if (arg == "-C" || arg == "-c") {
                STACK *newStack = new STACK(*stack);
                stack = newStack;

                cout << "C";
                if(int(*stack)) {
                    cout << "  ";
                    stack->print();
                }
            } else if (arg == "-A" || arg == "-a") {
                CHECK_IF_LAST_ARG

                arg = string(argv[++i]);
                if (! convertToInt(arg, number)) {
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                STACK *newStack = new STACK(number);
                *newStack = *stack;
                stack = newStack;

                cout << "A";
                if(int(*stack)) {
                    cout << "  ";
                    stack->print();
                }

            } else if (arg == "-N" || arg == "-n") {
                cout << "N  " << int(*stack);

            } else if (arg == "-G" || arg == "-g") {
                CHECK_IF_LAST_ARG

                arg = string(argv[++i]);
                if (! convertToInt(arg, number)) {
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                cout << "G  " << (*stack)[number];

            } else {
                cerr << "Wrong parameter: " << arg << endl;
                return 1;
            }

            if(i != argc - 1)
                cout << "  ";
        }
    } catch (logic_error error) {
        cout << "  E" << endl;
        return 1;
    }
    cout.rdbuf(coutBackup);
    outFile.close();
    return 0;
}

STACK::STACK(int m) : elems(new int [m]), max(m) {
    pos = 0;
}

STACK::STACK(const STACK &s) : elems(new int[s.max]), max(s.max) {
    pos = s.pos;
    for(int i=0; i < pos; ++i)
        elems[i] = s.elems[i];
}

int STACK::size() const {
    return max;
}

STACK::operator int() const {
    return pos;
}

int STACK::operator [](int x) const {
    if(x >= pos)
        throw std::logic_error("array index out of range");
    return elems[x];
}

STACK& STACK::operator <<(int e) {
    if(pos == max)
        throw std::logic_error("array is full, cannot push");
    elems[pos] = e;
    ++pos;
    return *this;
}

STACK& STACK::operator >>(int &e) {
    if(pos == 0)
        throw std::logic_error("pop from empty stack");
    e = elems[--pos];
    return *this;
}

STACK& STACK::operator =(const STACK &s) {
    // Do never use this in your code, this is a horrible example
    int *ref = const_cast<int *>(elems);
    delete ref;
    ref = new int [s.max];
    int *num = const_cast<int *>(&max);
    *num = s.max;
    pos = s.pos;

    for(int i=0; i < pos; ++i)
        elems[i] = s.elems[i];
    return *this;
}

void STACK::print() const {
    using namespace std;
    for(int i=0; i < pos; ++i) {
        if (i != pos - 1)
            cout << elems[i] << "  ";
        else
            cout << elems[i];
    }
    cout << flush;
}

STACK::~STACK(){
    delete []elems;
}

bool Utils::convertToInt(const std::string &s, int &out) {
    using namespace std;

    try {
        out = stoi(s);
    } catch (invalid_argument i) {
        return false;
    } catch (out_of_range o) {
        return false;
    }
    return true;
}

/**
 * @file U201514898_1.cpp
 *
 * @brief procedual-oriented stack implementation.
 *
 * @details
 * This file implemented a int stack and some
 * general method to operate it, developed using
 * procedual oriented method. A main function is
 * added to test the corectness of the stack
 *
 * @date 2017-10-23
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

#include<iostream>
#include<exception>
#include<climits>
#include<new>
#include<string>
#include<fstream>

/// @brief the stack structure
typedef struct _STACK{
    /// @brief the elements set
    int *elements;
    /// @brief the stack's capacity
    int max;
    /// @brief the stack's size
    int pos;
}STACK;

/// @brief initialize stack with size
/// @param[in] p the stack pointer
/// @param[in] m the size of the stack
void initSTACK(STACK *const p, int m);

/// @brief initialize a stack with another stack
/// @param[in] p the stack pointer to initialize
/// @param[in] s the stack to copy
void initSTACK(STACK *const p, const STACK &s);

/// @brief return the capacity of the stack
/// @param[in] p the stack pointer
/// @return the capacity of the stack
int size (const STACK *const p);

/// @brief return the element number of the stack
/// @param[in] p the stack pointer
/// @return the element number of the stack
int howMany (const STACK *const p);

/// @brief get the element from the stack
/// @param[in] p the stack pointer
/// @param[in] x the index of the element
/// @return the element
int getelem (const STACK *const p, int x);

/// @brief push an element to a stack
/// @param[in] p the the stack pointer
/// @param[in] e the element to push
/// @return the stack pointer
STACK *const push(STACK *const p, int e);

/// @brief pop an element from a stack
/// @param[in]  p the stack pointer
/// @param[out] e the element to pop
/// @return the stack pointer
STACK *const pop(STACK *const p, int &e);

/// @brief assign the elements in s to elements in p
/// @param[in] p the stack to assign
/// @param[in] s the stack to copy from
/// @return the stack after assigned
STACK *const assign(STACK*const p, const STACK&s);

/// @brief print elements in stack
/// @param[in] p the stack pointer
void print(const STACK*const p);

/// @brief de-construct the stack
/// @param[in] p the stack pointer
void destroySTACK(STACK*const p);

/// @brief return the capacity of the stack
/// @param[in] p the stack pointer
/// @return the capacity of the stack
int  size (const STACK *const p);

/// @brief return the element number of the stack
/// @param[in] p the stack pointer
/// @return the element number of the stack
int  howMany (const STACK *const p);

/// @brief get the element from the stack
/// @param[in] p the stack pointer
/// @param[in] x the index of the element
/// @return the element
int  getelem (const STACK *const p, int x);

/// @brief push an element to a stack
/// @param[in] p the the stack pointer
/// @param[in] e the element to push
/// @return the stack pointer
STACK *const push(STACK *const p, int e);

/// @brief pop an element from a stack
/// @param[in] p the stack pointer
/// @param[in] e the element to pop
/// @return the stack pointer
STACK *const pop(STACK *const p, int &e);

/// @brief assign the elements in s to elements in p
/// @param[in] p the stack to assign
/// @param[in] s the stack to copy from
/// @return the stack after assigned
STACK *const assign(STACK*const p, const STACK&s);

/// @brief print elements in stack
/// @param[in] p the stack pointer
void print(const STACK*const p);

/// @brief de-construct the stack
/// @param[in] p the stack pointer
void destroySTACK(STACK*const p);

namespace Utils{
    /// @brief convert string to int
    /// @param[in] s the string to convert
    /// @param[out] out the output int
    /// @return if the conversion is successful
    bool convertToInt(const std::string &s, int &out);
}

int main(int argc, char *argv[]){
    using namespace std;
    using namespace Utils;

    // arguments
    string arg;
    int number;

    // stack for test
    STACK *stack = new STACK;

    // file to write
    ofstream outFile;
    outFile.open("U201514898_1.TXT");

    auto coutBackup = cout.rdbuf();
    cout.rdbuf(outFile.rdbuf());

#define CHECK_IF_LAST_ARG if (i == argc - 1){ \
    cerr << "paramter not found after: " << arg << endl; \
    return 1; \
}

    try{
        for(int i=1; i < argc; ++i){
            arg = string(argv[i]);
            if (arg == "-S" || arg == "-s"){
                CHECK_IF_LAST_ARG;

                arg = string(argv[++i]);
                if (! convertToInt(arg, number)){
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                initSTACK(stack, number);
                cout << "S  " << number;
            } else if (arg == "-I" || arg == "-i") {
                cout << "I";

                for(++i; i < argc; ++i){
                    arg = string(argv[i]);
                    // until there's noting to covert
                    if(!convertToInt(arg, number)){
                        break;
                    }
                    push(stack, number);
                }
                if(i != argc)
                    --i;

                if(howMany(stack)){
                    cout << "  ";
                    print(stack);
                }
            } else if (arg == "-O" || arg == "-o") {
                CHECK_IF_LAST_ARG

                cout << "O";
                arg = string(argv[++i]);
                if (! convertToInt(arg, number)){
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                int garbage;
                for(int j=0; j<number; ++j){
                    pop(stack, garbage);
                }

                if(howMany(stack)) {
                    cout << "  ";
                    print(stack);
                }

            } else if (arg == "-C" || arg == "-c") {
                STACK *newStack = new STACK;
                initSTACK(newStack, *stack);
                stack = newStack;

                cout << "C";
                if(howMany(stack)) {
                    cout << "  ";
                    print(stack);
                }
            } else if (arg == "-A" || arg == "-a") {
                CHECK_IF_LAST_ARG

                arg = string(argv[++i]);
                if (! convertToInt(arg, number)){
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                STACK *newStack = new STACK;
                initSTACK(newStack, number);
                assign(newStack, *stack);
                stack = newStack;

                cout << "A";
                if(howMany(stack)) {
                    cout << "  ";
                    print(stack);
                }

            } else if (arg == "-N" || arg == "-n") {
                cout << "N  " << howMany(stack);

            } else if (arg == "-G" || arg =="-g"){
                CHECK_IF_LAST_ARG

                arg = string(argv[++i]);
                if(! convertToInt(arg, number)){
                    cout << "Error: cannot convert " << arg << " to int" << endl;
                }

                cout << "G  " << getelem(stack, number);
            } else {
                cerr << "Wrong parameter: " << arg << endl;
                return 1;
            }

            if(i != argc - 1)
                cout << "  ";
        }
    } catch (logic_error err) {
        cout << "  E";
        return 1;
    }
    cout.rdbuf(coutBackup);
    outFile.close();
    return 0;
}

void initSTACK(STACK *const p, int m){
    p->elements = new int [m];
    p->max = m;
    p->pos = 0;
}

void initSTACK(STACK *const p, const STACK &s){
    p->elements = new int [s.max];
    p->pos = s.pos;
    for(int i=0; i<s.pos; ++i)
        p->elements[i] = s.elements[i];
}

int size (const STACK *const p){
    return p->max;
}

int howMany (const STACK *const p){
    return p->pos;
}

int getelem (const STACK *const p, int x){
    using namespace std;
    if(x >= p->max)
        throw logic_error("index overflow");
    return p->elements[x];
}

STACK *const push(STACK *const p, int e){
    using namespace std;

    // if space is not enough
    if(p->pos == p->max)
        throw logic_error("push: stack size not enough");

    p->elements[p->pos] = e;
    ++p->pos;
    return p;
}

STACK *const pop(STACK *const p, int &e){
    using namespace std;

    // if stack is empty
    if(p->pos == 0)
        throw logic_error("pop: pop from a empty stack");

    // pop the elements
    e = p->elements[--p->pos];
    return p;
}

STACK *const assign(STACK*const p, const STACK&s){
    // copy properties
    p->pos = s.pos;
    p->max = s.max;

    // re-alloc space
    delete p->elements;
    p->elements = new int [p->pos];

    // copy elements
    for(int i=0; i<s.pos; ++i)
        p->elements[i] = s.elements[i];

    return p;
}

void print(const STACK *const p){
    using namespace std;
    for(int i=0; i < p->pos; ++i){
        if (i != p->pos - 1)
            cout << p->elements[i] << "  ";
        else
            cout << p->elements[i];
    }
    cout << flush;
}

void destroySTACK(STACK*const p){
    delete [] p->elements;
    p->elements = nullptr;
    p->pos = 0;
    p->max = 0;
}

bool Utils::convertToInt(const std::string &s, int &out){
    using namespace std;

    try {
        out = stoi(s);
    } catch (invalid_argument i){
        return false;
    } catch (out_of_range o){
        return false;
    }
    return true;
}

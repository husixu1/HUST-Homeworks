/**
 * @file U201514898_4.cpp
 * @brief object-oriented queue implementation, opertor-overloaded version.
 *
 * @details
 * Again, this file implemented a int queue and some
 * overloaded operators to operate it, developed using
 * added to test the correctness and robustness of the queue.
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

/// @brief the queue class
class QUEUE {
private:
    /// @brief the array used to store queue elements
    int *const elems;
    /// @brief queue capacity
    const int max;
    /// @brief queue size
    int head, tail;

public:
    /// @brief constructor, initialize the queue with the size of m
    /// @param[in] m the size of the queue
    QUEUE(int m);

    /// @brief copy-constructor, initialize the queue with another queue
    /// @param[in] s the queue to copy
    QUEUE(const QUEUE &s);

    /// @brief get if the queue is full
    /// @return if the queue is full
    virtual int full() const;

    /// @brief get the size of the queue
    /// @return queue size
    virtual operator int() const;

    /// @brief get the element with index x
    /// @param[in] x the element index to get
    /// @return the element with index x
    virtual int operator [](int x) const;

    /// @brief push an element to the queue
    /// @param[in] e the element to push
    /// @return the queue after pushing
    virtual QUEUE& operator <<(int e);

    /// @brief pop an element from the queue
    /// @param[out] e the poped element
    /// @return the queue after poping
    virtual QUEUE& operator >>(int &e);

    /// @brief copya a queue to this queue
    /// @param s the queue to copy
    /// @return the copied queue
    /// @note the current queue's element will be deleted
    virtual QUEUE& operator =(const QUEUE &s);

    /// @brief print the elements of the thread
    virtual void print() const;

    /// @brief de-constructor, destroy the queue
    virtual ~QUEUE();
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

    // queue for test
    QUEUE *queue = nullptr ;

    // file to write
    ofstream outFile;
    outFile.open("U201514898_4.TXT");

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

                queue = new QUEUE(number);
                cout << "S  " << number;
            } else if (arg == "-I" || arg == "-i") {
                cout << "I";

                for(++i; i < argc; ++i) {
                    arg = string(argv[i]);
                    // until there's noting to covert
                    if(!convertToInt(arg, number)) {
                        break;
                    }
                    (*queue) << number;
                }
                if(i != argc)
                    --i;

                if(int(*queue)) {
                    cout << "  ";
                    queue->print();
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
                    (*queue) >> garbage;
                }

                if(int(*queue)) {
                    cout << "  ";
                    queue->print();
                }

            } else if (arg == "-C" || arg == "-c") {
                QUEUE *newQueue = new QUEUE(*queue);
                queue = newQueue;

                cout << "C";
                if(int(*queue)) {
                    cout << "  ";
                    queue->print();
                }
            } else if (arg == "-A" || arg == "-a") {
                CHECK_IF_LAST_ARG

                arg = string(argv[++i]);
                if (! convertToInt(arg, number)) {
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                QUEUE *newQueue = new QUEUE(number);
                *newQueue = *queue;
                queue = newQueue;

                cout << "A";
                if(int(*queue)) {
                    cout << "  ";
                    queue->print();
                }

            } else if (arg == "-N" || arg == "-n") {
                cout << "N  " << int(*queue);

            } else if (arg == "-G" || arg == "-g") {
                CHECK_IF_LAST_ARG

                arg = string(argv[++i]);
                if (! convertToInt(arg, number)) {
                    cerr << "Error: cannot convert " << arg << " to int" << endl;
                    return 1;
                }

                cout << "G  " << (*queue)[number];

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

QUEUE::QUEUE(int m) : elems(new int [m]), max(m) {
    head = 0;
    tail = 0;
}

QUEUE::QUEUE(const QUEUE &s) : elems(new int[s.max]), max(s.max) {
    head = s.head;
    tail = s.tail;
    for(int i=0; i < s.max; ++i)
        elems[i] = s.elems[i];
}

QUEUE::operator int() const {
    return tail >= head ? tail - head : tail + max - head;
}

int QUEUE::full() const {
    return tail == head-1 || (head == 0 && tail == max - 1);
}

int QUEUE::operator [](int x) const {
    if ((head <= tail && (x < head || tail <= x)) ||
        (head > tail && (tail <= x && x < head)))
        throw std::logic_error("array index out of range");
    return elems[x];
}

QUEUE& QUEUE::operator <<(int e) {
    if(tail == head-1 || (head == 0 && tail == max - 1))
        throw std::logic_error("queue is full, cannot push");
    if(tail == max - 1){
        elems[tail] = e;
        tail = 0;
    } else {
        elems[tail++] = e;
    }
    return *this;
}

QUEUE& QUEUE::operator >>(int &e) {
    // throw exception if empty
    if(head == tail)
        throw std::logic_error("poping from empty queue");
    if(head == max){
        e = elems[head];
        head = 0;
    } else {
        e = elems[++head];
    }
    return *this;
}

QUEUE& QUEUE::operator =(const QUEUE &s) {
    // Do never use this in your code, this is a horrible example
    int *ref = const_cast<int *>(elems);
    delete ref;
    ref = new int [s.max];
    int *num = const_cast<int *>(&max);
    *num = s.max;
    head = s.head;
    tail = s.tail;

    for(int i=0; i < max; ++i)
        elems[i] = s.elems[i];
    return *this;
}

void QUEUE::print() const {
    using namespace std;
    for(int i = head; i != tail; i = ((i + 1) % max)) {
        if (i != head)
            cout << "  ";
        cout << elems[i];
    }
    cout << flush;
}

QUEUE::~QUEUE(){
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

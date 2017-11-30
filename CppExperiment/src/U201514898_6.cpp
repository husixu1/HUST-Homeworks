/**
 * @file U201514898_6.cpp
 * @brief object-oriented queue implementation, two stack inherited version.
 *
 * @details
 * This file implemented a int queue using two stacks, inheriting one of them
 * and some overloaded operators to operate it,
 * developed using object oriented method. A main function is
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
#include<limits>

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
    virtual STACK &operator <<(int e);

    /// @brief pop an element from the stack
    /// @param[out] e the poped element
    /// @return the stack after poping
    virtual STACK &operator >>(int &e);

    /// @brief copy a stack to this stack
    /// @param s the stack to copy
    /// @return the copied stack
    /// @note the current stack's element will be deleted
    virtual STACK &operator =(const STACK &s);

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

/// @brief judge if a int is a prime number
/// @param[in] a the int to judge
/// @return is a a prime number
bool isPrime(const int a);
}

/// @brief the queue class, using two stack
class QUEUE : public STACK {
private:
    /// @brief the two stack used to construct the queue
    STACK s2;

public:
    /// @brief constructor, initialize the queue with the size of m
    /// @param[in] m the size of the queue
    QUEUE(int m);

    /// @brief copy-constructor, initialize the queue with another queue
    /// @param[in] s the queue to copy
    QUEUE(const QUEUE &s);

    /// @brief get the capacity of the queue
    /// @return queue capacity
    virtual operator int() const;

    /// @brief get the size of the queue
    /// @return queue size
    virtual int full() const;

    /// @brief get the element with index x
    /// @param[in] x the element index to get
    /// @return the element with index x
    virtual int operator[](int x)const;

    /// @brief push an element to the queue
    /// @param[in] e the element to push
    /// @return the queue after pushing
    virtual QUEUE &operator<<(int e);

    /// @brief pop an element from the queue
    /// @param[out] e the poped element
    /// @return the queue after poping
    virtual QUEUE &operator>>(int &e);

    /// @brief copy a queue to this queue
    /// @param s the queue to copy
    /// @return the copied queue
    /// @note the current queue's element will be deleted
    virtual QUEUE &operator=(const QUEUE &s);

    /// @brief print the elements of the thread
    virtual void print() const;

    /// @brief de-constructor, destroy the queue
    virtual ~QUEUE();
};

int main(int argc, char *argv[]) {
    using namespace std;
    using namespace Utils;

    // if argc is 1, run dancer problem, else run tests
    if (argc == 1) {
        int menNum, womenNum, m, f;
        //> get men number
        while (true) {
            cout << "Please enter the number of gentlemans: " << flush;
            cin  >> menNum;
            if (cin.eof() || cin.bad()) {
                continue;
            } else if (cin.fail()) {
                cout << "--> Input not recognized." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (!isPrime(menNum)) {
                cout << "--> This is not a prime number." << endl;
            } else
                break;
        }
        //> get women number
        while (true) {
            cout << "Please enter the number of laydies: " << flush;
            cin  >> womenNum;
            if (cin.eof() || cin.bad()) {
                continue;
            } else if (cin.fail()) {
                cout << "--> Input not recognized." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (!isPrime(womenNum)) {
                cout << "--> This is not a prime number." << endl;
            } else if (womenNum == menNum) {
                cout << "--> The number of laydies must be unequal with the number of gentlemans." << endl;
            } else
                break;
        }
        //> get m
        while (true) {
            cout << "Please enter m (1 <= m <= number of gentlemans): " << flush;
            cin >> m;
            if (cin.eof() || cin.bad()) {
                continue;
            } else if (cin.fail()) {
                cout << "--> Input not recognized." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (m <= 0 || m > menNum) {
                cout << "--> m out of range" << endl;
            } else
                break;
        }
        //> get f
        while (true) {
            cout << "Please enter f (1 <= f <= number of ladies): " << flush;
            cin >> f;
            if (cin.eof() || cin.bad()) {
                continue;
            } else if (cin.fail()) {
                cout << "--> Input not recognized." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else if (f <= 0 || f > womenNum) {
                cout << "--> f out of range" << endl;
            } else
                break;
        }

        // create QUEUE
        QUEUE queueMen(2 * menNum + 1), queueWomen(2 * womenNum + 1);
        for (int i = 0; i < menNum; ++i)
            queueMen << i + 1;
        for (int i = 0; i < womenNum; ++i)
            queueWomen << i + 1;

        int currentMan, currentWoman, counter = 1;
        while (1) {
            queueMen >> currentMan;
            queueWomen >> currentWoman;
            if (currentMan == m && currentWoman == f) {
                cout << "==> m and f met at round " << counter << endl;
                break;
            }
            queueMen << currentMan;
            queueWomen << currentWoman;
            ++counter;
        }
        return 0;
    } else {
        // arguments
        string arg;
        int number;

        // queue for test
        QUEUE *queue = nullptr ;

        // file to write
        ofstream outFile;
        outFile.open("U201514898_6.TXT");

        auto coutBackup = cout.rdbuf();
        cout.rdbuf(outFile.rdbuf());

        #define CHECK_IF_LAST_ARG if (i == argc - 1){ \
            cerr << "paramter not found after: " << arg << endl; \
            return 1; \
        }

        try {
            for (int i = 1; i < argc; ++i) {
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

                    for (++i; i < argc; ++i) {
                        arg = string(argv[i]);
                        // until there's noting to covert
                        if (!convertToInt(arg, number)) {
                            break;
                        }
                        (*queue) << number;
                    }
                    if (i != argc)
                        --i;

                    if (int(*queue)) {
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
                    for (int j = 0; j < number; ++j) {
                        (*queue) >> garbage;
                    }

                    if (int(*queue)) {
                        cout << "  ";
                        queue->print();
                    }

                } else if (arg == "-C" || arg == "-c") {
                    QUEUE *newQueue = new QUEUE(*queue);
                    queue = newQueue;

                    cout << "C";
                    if (int(*queue)) {
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
                    if (int(*queue)) {
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

                if (i != argc - 1)
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
}

// STACK implementation
STACK::STACK(int m) : elems(new int [m]), max(m) {
    pos = 0;
}

STACK::STACK(const STACK &s) : elems(new int[s.max]), max(s.max) {
    pos = s.pos;
    for (int i = 0; i < pos; ++i)
        elems[i] = s.elems[i];
}

int STACK::size() const {
    return max;
}

STACK::operator int() const {
    return pos;
}

int STACK::operator [](int x) const {
    if (x >= pos)
        throw std::logic_error("array index out of range");
    return elems[x];
}

STACK &STACK::operator <<(int e) {
    if (pos == max)
        throw std::logic_error("array is full, cannot push");
    elems[pos] = e;
    ++pos;
    return *this;
}

STACK &STACK::operator >>(int &e) {
    if (pos == 0)
        throw std::logic_error("pop from empty stack");
    e = elems[--pos];
    return *this;
}

STACK &STACK::operator =(const STACK &s) {
    // Do never use this in your code, this is a horrible example
    int *ref = const_cast<int *>(elems);
    delete ref;
    ref = new int [s.max];
    int *num = const_cast<int *>(&max);
    *num = s.max;
    pos = s.pos;

    for (int i = 0; i < pos; ++i)
        elems[i] = s.elems[i];
    return *this;
}

void STACK::print() const {
    using namespace std;
    for (int i = 0; i < pos; ++i) {
        if (i != pos - 1)
            cout << elems[i] << "  ";
        else
            cout << elems[i];
    }
    cout << flush;
}

STACK::~STACK() {
    delete []elems;
}

// QUEUE implementation
QUEUE::QUEUE(int m) : STACK(m), s2(m) {
    return;
}

QUEUE::QUEUE(const QUEUE &s) : STACK(static_cast<STACK>(s)), s2(s.s2) {
    return;
}

QUEUE::operator int() const {
    return STACK::operator int() + s2.operator int();
}

int QUEUE::full() const {
    return STACK::operator int() == STACK::size() && !s2.operator int();
}

int QUEUE::operator [](int x) const {
    if (x >= STACK::operator int() + s2.operator int())
        throw std::logic_error("trying to get elem outside of queue");

    // judge where is the element
    if (x >= int(s2))
        return STACK::operator [](x - int(s2));
    else
        return s2[int(s2) - x - 1];
}

QUEUE &QUEUE::operator <<(int e) {
    // judge if s1 is full
    if (STACK::operator int() == STACK::size()) {
        int temp;
        int origSize = STACK::operator int();
        for (int i = 0; i < origSize; ++i) {
            STACK::operator >>(temp);
            s2 << temp;
        }
    }
    // push into stack
    STACK::operator <<(e);
    return *this;
}

QUEUE &QUEUE::operator >>(int &e) {
    if (int(s2)) {
        s2 >> e;
    } else {
        int temp;
        int origSize = STACK::operator int();
        for (int i = 0; i < origSize - 1; ++i) {
            STACK::operator >>(temp);
            s2 << temp;
        }
        STACK::operator >>(e);
    }
    return *this;
}

QUEUE &QUEUE::operator =(const QUEUE &s) {
    STACK::operator=(static_cast<STACK>(s));
    s2 = s.s2;
    return *this;
}

void QUEUE::print() const {
    using namespace std;

    // print elements in s2
    for (int i = 0; i < int(s2); ++i) {
        if (i)
            cout << "  ";
        cout << s2[int(s2) - i - 1];
    }

    if (int(s2) && STACK::operator int())
        cout << "  ";

    // print elements in s1
    for (int i = 0; i < STACK::operator int(); ++i) {
        if (i)
            cout << "  ";
        cout << STACK::operator [](i);
    }

    cout << flush;
}

QUEUE::~QUEUE() {
    return;
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

bool Utils::isPrime(const int a) {
    if (a <= 1)
        return false;

    int i;
    for (i = 2; i * i <= a; i++)
        if (a % i == 0)
            return false;
    return true;
}


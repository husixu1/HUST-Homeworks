#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

/// @brief add two big number
/// @param a one big number
/// @param b another big number
/// @return a+b
string bigNumAdd(const string &a, const string &b);

/// @brief substract big number
/// @param a one big number
/// @param b another big number
/// @return a-b
string bigNumSub(const string &a, const string &b);

/// @brief multiply two big number
/// @param a one big number
/// @param b another big number
/// @return a*b
string bigNumMult(const string &a, const string &b);

/// @brief add two unsigned big number
/// @param a one unsigned big number
/// @param b another unsigned big number
/// @return a+b
string bigNumAddHelper(const string &a, const string &b);

/// @brief substract two unsigned big number, and a must be bigger than b
/// @param a one unsigned big number
/// @param b another unsigned big number, less than a
/// @return a-b
string bigNumSubHelper(const string &a, const string &b);

/// @brief multiply two unsigned big number using karatsuba
/// @param a one unsigned big number
/// @param b another unsigned big number
/// @return a*b
string bigNumMultHelper(const string &a, const string &b);

/// @brief compair two unsigned big number
/// @param a one unsigned big number
/// @param b another unsigned big number
/// @return (a < b)
bool bigNumCompareHelper(const string &a, const string &b);

/// @brief return the negative of a negative big number
/// @param a a negative number, must start with '-'
/// @return -a
inline string negative(const string &a) {
    return string(a.begin() + 1, a.end());
}

int main() {
    unsigned casenNum;
    ifstream inFile;
    ofstream outFile;
    inFile.open("in.dat");
    outFile.open("out.dat");

    // read case numbers
    inFile >> casenNum;

    // calculate each case
    for (unsigned i = 0; i < casenNum; ++i) {
        string a, b;
        int op;

        // read data
        inFile >> a >> b >> op;

        // do operation
        // TODO: deal with negative number
        switch (op) {
            case 1:
                outFile << bigNumAdd(a, b) << endl;
                break;
            case 2:
                outFile << bigNumSub(a, b) << endl;
                break;
            case 3:
                outFile << bigNumMult(a, b) << endl;
                break;
            default:
                break;
        }
    }

    inFile.close();
    outFile.close();
    return 0;
}

string bigNumAdd(const string &a, const string &b) {
    bool isANegative = false, isBNeagtive = false;
    string result;

    if (a.front() == '-')
        isANegative = true;
    if (b.front() == '-')
        isBNeagtive = true;

    if (!isANegative && !isBNeagtive)
        result =  bigNumAddHelper(a, b);

    if (isANegative && isBNeagtive)
        result =  "-" + bigNumAddHelper(negative(a), negative(b));

    if (isANegative && !isBNeagtive) {
        // |a| < b ?
        if (bigNumCompareHelper(negative(a), b)) {
            result =  bigNumSubHelper(b, negative(a));
        } else {
            result =  "-" + bigNumSubHelper(negative(a), b);
        }
    }

    if (!isANegative && isBNeagtive) {
        // |b| < a?
        if (bigNumCompareHelper(negative(b), a)) {
            result =  bigNumSubHelper(a, negative(b));
        } else {
            result =  "-" + bigNumSubHelper(negative(b), a);
        }
    }
    if (result == "-0")
        return "0";
    return result;
}

string bigNumSub(const string &a, const string &b) {
    if (b.front() == '-') {
        return bigNumAdd(a, negative(b));
    } else {
        return bigNumAdd(a, "-" + b);
    }
}

string bigNumAddHelper(const string &a, const string &b) {
    int carry = 0;
    string result;

    int indexA = a.size() - 1, indexB = b.size() - 1, actual;
    for (; indexA >= 0  && indexB >= 0 ; --indexA, --indexB) {
        actual = carry + static_cast<int>(a[indexA]) - '0' + static_cast<int>(b[indexB]) - '0';
        result.push_back(actual % 10 + '0');
        carry = actual / 10;
    }
    for (; indexA >= 0; --indexA) {
        actual = carry + static_cast<int>(a[indexA]) - '0';
        result.push_back(actual % 10 + '0');
        carry = actual / 10;
    }
    for (; indexB >= 0; --indexB) {
        actual = carry + static_cast<int>(b[indexB]) - '0';
        result.push_back(actual % 10 + '0');
        carry = actual / 10;
    }
    if (carry)
        result.push_back(carry + '0');

    while (result.size() && result.back() == '0')
        result.pop_back();
    if (result.empty())
        result.push_back('0');
    reverse(result.begin(), result.end());
    return result;
}

string bigNumMult(const string &a, const string &b) {
    bool isNegative = false;
    string absA, absB;
    string result;

    if (a.front() == '-') {
        isNegative = !isNegative;
        absA = negative(a);
    } else {
        absA = a;
    }

    if (b.front() == '-') {
        isNegative = !isNegative;
        absB = negative(b);
    } else {
        absB = b;
    }

    return (isNegative ? "-" : "") + bigNumMultHelper(absA, absB);
}

string bigNumSubHelper(const string &a, const string &b) {
    int borrow = 0;
    string result;

    int indexA = a.size() - 1, indexB = b.size() - 1, actual;
    for (; indexA >= 0 && indexB >= 0; --indexA, --indexB) {
        actual = a[indexA] - b[indexB] - borrow;
        result.push_back((actual + 10) % 10 + '0');
        borrow = actual < 0 ? 1 : 0;
    }
    for (; indexA >= 0; --indexA) {
        actual = a[indexA] - '0' - borrow;
        result.push_back((actual + 10) % 10 + '0');
        borrow = actual < 0 ? 1 : 0;
    }

    while (result.size() &&  result.back() == '0')
        result.pop_back();
    if (result.empty())
        result.push_back('0');
    reverse(result.begin(), result.end());
    return result;
}

string bigNumMultHelper(const string &a, const string &b) {
    if (a.size() == 1) {
        string result("0");
        for (int i = 0; i < a[0] - '0'; ++i)
            result = bigNumAdd(result, b);
        return result;
    } else if (b.size() == 1) {
        string result("0");
        for (int i = 0; i < b[0] - '0'; ++i)
            result = bigNumAdd(result, a);
        return result;
    } else if (a.size() == 0 || b.size() == 0) {
        return "";
    }

    unsigned halfSize = a.size() > b.size() ? a.size() / 2 : b.size() / 2;

    string high1(a.begin(), (halfSize > a.size() ? a.begin() : a.end() - halfSize));
    string low1((halfSize > a.size() ? a.begin() : a.end() - halfSize), a.end());
    string high2(b.begin(), (halfSize > b.size() ? b.begin() : b.end() - halfSize));
    string low2((halfSize > b.size() ? b.begin() : b.end() - halfSize), b.end());

    string z0 = bigNumMultHelper(low1, low2);
    string z1 = bigNumMultHelper(bigNumAdd(low1, high1), bigNumAdd(low2, high2));
    string z2 = bigNumMultHelper(high1, high2);

    string temp1 = z2, temp2 = bigNumSub(bigNumSub(z1, z2), z0);
    for (unsigned i = 0; i < 2 * halfSize; ++i)
        temp1.push_back('0');
    for (unsigned i = 0; i < halfSize; ++i)
        temp2.push_back('0');
    return bigNumAdd(bigNumAdd(temp1, temp2), z0);
}

bool bigNumCompareHelper(const string &a, const string &b) {
    unsigned startA = 0, startB = 0;
    for (; startA < a.size() && a[startA] == '0'; ++startA);
    for (; startB < b.size() && b[startB] == '0'; ++startB);

    if (a.size() - startA < b.size() - startB)
        return true;
    if (a.size() - startA > b.size() - startB)
        return false;

    for (; startA < a.size(); ++startA, ++startB) {
        if (a[startA] < b[startB])
            return true;
        if (a[startA] > b[startB])
            return false;
    }
    return false;
}

#include<iostream>
#include<vector>
#include<cstring>

using namespace std;

const long long mod = 100000000;

// table[i][j] is the possibility when line i is at status j
// (only take former i lines into consideration)
unsigned long long table[12][1 << 12];
int infertiled[12];

inline bool isValid(int i, int s) {
    return !((s & (s << 1)) || (s & infertiled[i]));
}

inline bool isAdjacent(int a, int b) {
    return a & b;
}

int main() {
    int m, n;
    cin >> n >> m;

    memset(table, 0, sizeof(table));
    memset(infertiled, 0, sizeof(infertiled));

    int bit;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            cin >> bit;
            infertiled[i] |= ((bit ^ 1) << j);
        }

    // initialize
    for (int status = 0; status < (1 << m); ++status)
            table[0][status] = isValid(0, status);

    // iterate
    for (int i = 1; i < n; ++i) {
        for (int status = 0; status < (1 << m); ++status) {
            table[i][status] = 0;

            if (!isValid(i, status))
                continue;

            for (int previous = 0; previous < (1 << m); ++previous)
                if(isValid(i-1, previous) && !isAdjacent(previous, status))
                    (table[i][status] += table[i - 1][previous]) %= mod;
        }
    }

    unsigned long long result = 0;
    for (int i = 0; i < (1 << m); ++i) {
        result += table[n - 1][i];
        result %= mod;
    }

    cout << result << endl;
    return 0;
}

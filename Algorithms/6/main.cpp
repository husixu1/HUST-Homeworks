#include<climits>
#include<iostream>
#include<vector>
#include<queue>
#include<utility>

using namespace std;

struct Pool {
    int index;
    int f;
    int d;
    friend bool operator< (Pool a, Pool b);
};
bool operator< (Pool a, Pool b) {
    if (a.f == b.f)
        return a.index > b.index;
    return a.f < b.f;
}

int main() {
    bool first = true;
    while (true) {
        int n, h, temp;
        vector<int> t;
        cin >> n;
        if (n == 0)
            break;
        vector<Pool> pools(n);
        cin >> h;
        h *= 12;
        for (int i = 0; i < n; ++i) {
            cin >> temp;
            pools[i].index = i;
            pools[i].f = temp;
        }
        for (int i = 0; i < n; ++i) {
            cin >> temp;
            pools[i].d = temp;
        }
        for (int i = 0; i < n - 1; ++i) {
            cin >> temp;
            t.push_back(temp);
        }

        vector<int> bestPath(n, 0);
        int bestResult = 0;
        // enumerate all possible terminus
        for (int terminus = 0; terminus < n; ++terminus) {
            // reduce interval time
            int hour = h;
            for (int i = 0; i < terminus; ++i)
                hour -= t[i];

            // init
            priority_queue<Pool> priQueue;
            for (int i = 0; i <= terminus; ++i)
                priQueue.push(pools[i]);

            vector<int> roundPath(n, 0);
            int roundResult = 0;

            // run simulation
            while (hour--) {
                // do move
                ++roundPath[priQueue.top().index];
                roundResult += priQueue.top().f;

                Pool tempPool = priQueue.top();
                tempPool.f -= priQueue.top().d;
                if (tempPool.f < 0)
                    tempPool.f = 0;
                priQueue.pop();
                priQueue.push(tempPool);
            }
            if (roundResult > bestResult) {
                bestResult = roundResult;
                bestPath = roundPath;
            }
        }
        if (first)
            first = false;
        else
            cout << endl;

        for (unsigned i = 0; i < bestPath.size(); ++i) {
            if (i)
                cout << ", ";
            cout << bestPath[i] * 5;
        }
        cout << endl <<  "Number of fish expected: " << bestResult << endl;
    }
    return 0;
}

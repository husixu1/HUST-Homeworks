#include<iostream>
#include<fstream>
#include<vector>
#include<utility>
#include<algorithm>
#include<climits>

using namespace std;
typedef pair<int, int> Dot;
typedef vector<Dot> DotList;

inline uint64_t distanceSquare(const Dot &dotA, const Dot &dotB) {
    return (dotA.first - dotB.first) * (dotA.first - dotB.first)
            + (dotA.second - dotB.second) * (dotA.second - dotB.second);
}

inline uint64_t distanceSquare(const int posA, const int posB){
    return (posA - posB) * (posA - posB);
}

/// @brief find nearest dots
/// @param dots dots
/// @param result the result
/// @return nearest distance's square
uint64_t findNearestDotPairs(const DotList &dots, DotList &result);

int main() {
    // open input file and output file
    ifstream inFile;
    inFile.open("in.dat");
    ofstream outFile;
    outFile.open("out.dat");

    // total num
    unsigned groupNum;
    inFile >> groupNum;

    // groups of test
    for (unsigned i = 0; i < groupNum; ++i) {
        unsigned dotNum;
        int x, y;
        DotList dots;
        inFile >> dotNum;

        // read dot axies
        for (unsigned i = 0; i < dotNum; ++i) {
            inFile >> x >> y;
            dots.push_back({x, y});
        }

        // sort according to y, then x
        stable_sort(dots.begin(), dots.end(), [&](const Dot & a, const Dot & b)->bool {
            return a.second < b.second;
        });
        stable_sort(dots.begin(), dots.end(), [&](const Dot & a, const Dot & b)->bool {
            return a.first < b.first;
        });


        // start calculating
        DotList result;
        findNearestDotPairs(dots, result);

        // output
        for (auto dot : result)
            outFile << dot.first << " " << dot.second << " ";
        outFile << endl;
    }
    inFile.close();
    outFile.close();
    return 0;
}

uint64_t findNearestDotPairs(const DotList &dots, DotList &result) {
    if (dots.size() < 2)
        return UINT64_MAX;

    // find left and right
    vector<pair<int, int>> resultLeft, resultRight, resultMiddle;
    uint64_t minDistanceL = findNearestDotPairs(DotList(dots.begin(), dots.begin() + dots.size() / 2), resultLeft);
    uint64_t minDistanceR = findNearestDotPairs(DotList(dots.begin() + dots.size() / 2, dots.end()), resultRight);
    uint64_t minDistanceM = minDistanceL < minDistanceR ? minDistanceL : minDistanceR;

    Dot midDot = dots[dots.size() / 2];

    // iterate through all dots
    for (unsigned i = 0; i < dots.size(); ++i) {
        // do not calculate dots left to the middle stripe
        if (dots[i].first < midDot.first
                && distanceSquare(dots[i].first, midDot.first) > minDistanceM)
            continue;

        // break if all middle dots reside at left part are calaulated (dots are sorted so yeah)
        if (dots[i].first >= midDot.first)
            break;

        // calculate through the righter half of the middle stripe
        for (unsigned j = i; j < dots.size(); ++j) {
            if (dots[j].first < midDot.first)
                continue;

            // restrict to minDistance * 2 minDistance rectangle
            if (distanceSquare(dots[j].second, dots[i].second) > minDistanceM)
                continue;

            // calculation finished
            if (dots[j].first > midDot.first &&
                    distanceSquare(dots[j].first, midDot.first) > minDistanceM)
                break;

            if (distanceSquare(dots[i], dots[j]) < minDistanceM) {
                // found a better one
                minDistanceM = distanceSquare(dots[i], dots[j]);
                resultMiddle.clear();
                resultMiddle.push_back(dots[i]);
                resultMiddle.push_back(dots[j]);
            } else if (distanceSquare(dots[i], dots[j]) == minDistanceM) {
                // found a equal one
                resultMiddle.push_back(dots[i]);
                resultMiddle.push_back(dots[j]);
            }
        }
    }
    // combine
    // minDistanceL can only >= minDistanceM
    if (minDistanceL == minDistanceM)
        resultMiddle.insert(resultMiddle.end(), resultLeft.begin(), resultLeft.end());
    // minDistanceR can only >= minDistanceM
    if (minDistanceR == minDistanceM)
        resultMiddle.insert(resultMiddle.end(), resultRight.begin(), resultRight.end());

    result = resultMiddle;
    return minDistanceM;
}

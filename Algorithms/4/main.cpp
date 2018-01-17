#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;
static const int Infinity = 100;
static const int InfinityPathLen = 32767;

/// @brief find all shortest path
/// @param matrix the input matrix
/// @param resultLen shortest len matrix
/// @param resultPath shortest path matrix
void allPaths(const vector<vector<int>> &matrix,
        vector<vector<int>> &resultLen,
        vector<vector<vector<int>>> &resultPath);

int main() {
    ifstream inFile;
    ofstream outFile;
    inFile.open("in.dat");
    outFile.open("out.dat");

    unsigned caseNum, matrixSize;
    inFile >> caseNum;
    outFile << caseNum << endl;
    // iterate all cases
    while (caseNum--) {
        inFile >> matrixSize;
        outFile << matrixSize << endl;
        vector<vector<int>> matrix(matrixSize, vector<int>(matrixSize, 0));
        vector<vector<int>> shortestLength(matrixSize, vector<int>(matrixSize, InfinityPathLen));
        vector<vector<vector<int>>> shortestPath(matrixSize, vector<vector<int>>(matrixSize, vector<int>()));
        // read input
        for (unsigned i = 0; i < matrixSize; ++i)
            for (unsigned j = 0; j < matrixSize; ++j)
                inFile >> matrix[i][j];

        // do calculation
        allPaths(matrix, shortestLength, shortestPath);

        // output len
        for (unsigned i = 0; i < matrixSize; ++i) {
            for (unsigned j = 0; j < matrixSize; ++j)
                outFile << shortestLength[i][j] << " ";
            outFile << endl;
        }
        // output path
        for (unsigned i = 0; i < matrixSize; ++i) {
            for (unsigned j = 0; j < matrixSize; ++j) {
                if (i == j)
                    outFile << i + 1;
                else if (shortestPath[i][j].size() == 0)
                    outFile << "NULL";
                else
                    for (auto node : shortestPath[i][j])
                        outFile << node + 1 << " ";
                outFile << endl;
            }
        }
    }

    inFile.close();
    outFile.close();
    return 0;
}

void allPaths(const vector<vector<int>> &matrix,
        vector<vector<int>> &resultLen,
        vector<vector<vector<int>>> &resultPath) {
    unsigned matrixSize = matrix.size();
    vector<vector<int>> next(matrixSize, vector<int>(matrixSize, 0));

    // initialization
    resultLen = matrix;
    for (unsigned i = 0; i < matrixSize; ++i)
        for (unsigned j = 0; j < matrixSize; ++j) {
            if (matrix[i][j] >= Infinity)
                next[i][j] = -1;
            else
                next[i][j] = j;
        }

    // Floyd-Warshall
    for (unsigned k = 0; k < matrixSize; ++k)
        for (unsigned i = 0; i < matrixSize; ++i)
            for (unsigned j = 0; j < matrixSize; ++j)
                if (resultLen[i][j] > resultLen[i][k] + resultLen[k][j]) {
                    resultLen[i][j] = resultLen[i][k] + resultLen[k][j];
                    next[i][j] = next[i][k];
                }

    // find path
    unsigned tempNode;
    for (unsigned i = 0; i < matrixSize; ++i) {
        for (unsigned j = 0; j < matrixSize; ++j) {
            if (next[i][j] == -1)
                continue;
            tempNode = i;
            resultPath[i][j].push_back(tempNode);
            while (tempNode != j) {
                tempNode = next[tempNode][j];
                resultPath[i][j].push_back(tempNode);
            }
            reverse(resultPath[i][j].begin(), resultPath[i][j].end());
        }
    }
}

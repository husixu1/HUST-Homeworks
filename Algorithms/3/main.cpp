#include<iostream>
#include<fstream>
#include<vector>
#include<array>

using namespace std;

/// @brief construct optimal BST
/// @param[in] root the root table
/// @param[in] i pass 0
/// @param[in] j pass root[0].size() - 1
/// @param[out] result a array of (i, j, d, m), in which d could only be 0, 1.
/// if d == 0 then Ki is the left child of Kj, else Ki is the right child of Kj,
/// m indicates if i is a fake node
/// @return something you should not care
int constructOptimalBST(const vector<vector<int>> &root, int i, int j, vector<array<int, 4>> &result);

static int fakeCounter;

int main() {
    ifstream inFile;
    ofstream outFile;

    inFile.open("in.dat");
    outFile.open("out.dat");

    unsigned matrixSize;
    inFile >> matrixSize;

    vector<vector<int>> root(matrixSize, vector<int>(matrixSize, 0));
    for (unsigned i = 0; i < matrixSize; ++i)
        for (unsigned j = 0; j < matrixSize; ++j)
            inFile >> root[i][j];

    vector<array<int, 4>> result;

    fakeCounter = -1;
    int rootNode = constructOptimalBST(root, 0, matrixSize - 1, result);

    outFile << string("k") << rootNode << string("是根") << endl;
    for (auto i : result) {
        outFile << string(i[3] ? "d" : "k") << (i[3] ? i[0] : i[0])
                << string("是k") << i[1]
                << string("的") << (i[2] ? string("右") : string("左"))
                << string("孩子") << endl;
    }

    inFile.close();
    outFile.close();
    return 0;
}

int constructOptimalBST(const vector<vector<int>> &root, int i, int j, vector<array<int, 4>> &result) {
    if (i > j){
        ++fakeCounter;
        return -1;
    }

    int child;
    // first insert right to front
    if((child = constructOptimalBST(root, root[i][j], j, result)) != -1)
        result.insert(result.begin(), {child, root[i][j], 1, 0});
    else
        result.insert(result.begin(), {static_cast<int>(root.size()) - fakeCounter, root[i][j], 1, 1});

    // then insert left to front
    if((child = constructOptimalBST(root, i, root[i][j] - 2,  result)) != -1)
        result.insert(result.begin(), {child, root[i][j], 0, 0});
    else
        result.insert(result.begin(), {static_cast<int>(root.size()) - fakeCounter, root[i][j], 0, 1});

    return root[i][j];
}

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <list>
#include <queue>
#include <utility>
#include <assert.h>

using namespace std;

class Board {
private:
    int board[81];
    inline int convert(int x, int y) {
        return y * 9 + x;
    }
public:
    Board() {
        for (int i = 0; i < 81; ++i)
            board[i] = 0;
    }
    Board(const Board &a) {
        for (int i = 0; i < 81; ++i)
            board[i] = a.board[i];
    }
    Board &operator=(const Board &a) {
        for (int i = 0; i < 81; ++i)
            board[i] = a.board[i];
        return *this;
    }

    /// @brief find the first empty cell
    /// @return (x, y), (-1, -1) if there's no empty cell
    std::pair<int, int> firstEmpty() {
        for (int i = 0; i < 81; ++i)
            if (board[i] == 0)
                return {i % 9, i / 9};
        return {-1, -1};
    }

    /// @brief set/reset value of one cell
    /// @param x x coordinate
    /// @param y y coordinate
    /// @param val value to set
    /// @return set/reset is successful
    bool set(int x, int y, int val) {
        if (board[convert(x, y)] == 0)
            board[convert(x, y)] = val;
        else if (val == 0)
            board[convert(x, y)] = 0;
        else
            return false;
        return true;
    }

    /// @brief return a list of possible value in this cell
    /// @details cell (x, y) is considered empty when calling this function
    /// @param x x coordinate
    /// @param y y coordinate
    /// @return a list of possible value
    std::list<int> possibleValue(int x, int y) {
        int hash[10] = {0}, i, j;
        for (i = (convert(x, y) / 9) * 9; i < (convert(x, y) / 9) * 9 + 9; ++i)
            hash[board[i]] = 1;
        for (i = convert(x, y) % 9; i < 81; i += 9)
            hash[board[i]] = 1;
        for (i = (convert(x, y) / 27) * 27 + ((convert(x, y) % 9) / 3) * 3,
                j = 0; j < 3; ++j, i += 6)
            for (int k = 0; k < 3; ++k, ++i)
                hash[board[i]] = 1;
        list<int> result = {};
        for (int i = 1; i < 10; ++i) {
            if (!hash[i])
                result.push_back(i);
        }
        return result;
    }

    inline int at(int x, int y) {
        return board[convert(x, y)];
    }

    inline int operator()(int x, int y) {
        return board[convert(x, y)];
    }

    /// @brief initialize board with string of '[1-9.]{81}'
    /// @param str the string with 81 characters
    bool initialize(std::string str) {
        if (str.size() != 81)
            return false;
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '.')
                board[i] = 0;
            else if ('1' <= str[i] <= '9')
                board[i] = str[i] - '0';
            else
                return false;
        }
        return true;
    }
    virtual ~Board() {}

#ifndef NDEBUG
    void prettyPrint() {
        cout << endl;
        for (int j = 0; j < 9; ++j) {
            for (int i = 0; i < 9; ++i) {
                cout << (*this)(i, j) << " ";
            }
            cout << endl;
        }
    }
#endif

}; // class Board

Board solve(Board &board) {
    // all solved
    pair<int, int> coordinate;
    if ((coordinate = board.firstEmpty()).first == -1)
        return board;

    list<int> possibleValues;
    if (!(possibleValues = board.possibleValue(coordinate.first, coordinate.second)).size())
        throw string("No solution");

    for (int i : possibleValues) {
        board.set(coordinate.first, coordinate.second, i);
        try {
            return solve(board);
        } catch (string s) {
            // no solution for this board
            if (s == string("No solution"));
        }
        // reset this cell
        board.set(coordinate.first, coordinate.second, 0);
    }

    // none subproblem has a solution
    throw string("No solution");
}

int main(int argc, char *argv[]) {
    ifstream dataFile;
    dataFile.open("../data/questions.txt");

    // the sudoku board
    Board board;

    // solve the problem
    string line, temp;
    ofstream outFile;
    outFile.open("solution.txt");
    for (int repeater = 0; repeater < 100; ++repeater) {
        temp.clear();
        // initialize board
        for (int i = 0; i < 9; ++i) {
            dataFile >> line;
            temp.append(line);
        }
        assert(board.initialize(temp));

        // solve board
        try{
            Board solution = solve(board);
            for(int i=0; i < 9; ++i){
                for(int j=0; j < 9; ++j){
                    outFile << solution(j, i);
                }
                outFile << endl;
            }
            outFile << endl;
#ifndef NDEBUG
            solution.prettyPrint();
#endif
            cout << endl << "Puzzle " << repeater << " solved." << endl;
        } catch(string){
            outFile << "No Answer" << endl;
            outFile << endl;
            cout << endl << "Puzzle " << repeater << " has no solution." << endl;
        }
    }
    dataFile.close();
    outFile.close();
    return 0;
}

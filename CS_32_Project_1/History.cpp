#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>

#include "City.h"
#include "History.h"

using namespace std;

History::History(int nRows, int nCols) 
    : m_row(nRows), m_col(nCols) {
    for (int r = 0; r < m_row; r++) {
        for (int c = 0; c < m_col; c++) {
            grid[r][c] = '.';
        }
    }
}

bool History::record(int r, int c) {
    if (r >= 1 && r <= m_row && c >= 1 && c <= m_col) {
        char currentPosition = grid[r - 1][c - 1];
        switch (currentPosition) {
        case '.':
            grid[r - 1][c - 1] = 'A';
            break;

        case 'Z':
            break;

        default:
            grid[r - 1][c - 1]++;
            break;
        }
        return true;
    }
    return false;
}

void History::display() const {
    clearScreen();
    for (int r = 0; r < m_row; r++) {
        for (int c = 0; c < m_col; c++) {
            cout << grid[r][c];
        }
        cout << endl;
    }
    cout << endl;
}

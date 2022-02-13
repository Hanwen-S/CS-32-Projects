#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"

using namespace std;

class City;

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;

private:
	int   m_row;
	int   m_col;
	char grid[MAXROWS][MAXCOLS];

};

#endif
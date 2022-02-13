#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <iostream>
#include <stack>

using namespace std;

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	struct Act
	{
		char m_value;
		Action m_act;
		int m_row;
		int m_col;
		int m_count;
		bool delTogether;
		bool isDelete;
	};

	stack<Act> ActStack;
};

#endif // STUDENTUNDO_H_

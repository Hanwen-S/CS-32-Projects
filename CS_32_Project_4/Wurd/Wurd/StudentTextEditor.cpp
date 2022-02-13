#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <list>

using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
	: TextEditor(undo) {
	m_row = 0;
	m_col = 0;
	lineList.clear();
}

StudentTextEditor::~StudentTextEditor()
{
	lineList.clear();
}

bool StudentTextEditor::load(std::string file) {
	ifstream infile(file);
	if (!infile) {
		return false;
	}
	reset();
	string curLine;
	while (getline(infile, curLine)) {
		if (curLine.size() > 2) {
			if (curLine.at(curLine.size() - 2) == '\r') {
				curLine.erase(curLine.size() - 2, 1);
			}
			else if (curLine.at(curLine.size() - 1) == '\r') {
				curLine.erase(curLine.size() - 1, 1);
			}
		}
		lineList.push_back(curLine);
	}
	return true;
}

bool StudentTextEditor::save(std::string file) {
	ofstream outfile(file);
	if (!outfile) {
		return false;
	}
	list<string>::iterator it;
	for (it = lineList.begin(); it != lineList.end(); ++it) {
		outfile << *it << '\n';
	}
	return true;
}

void StudentTextEditor::reset() {
	m_row = 0;
	m_col = 0;
	//clear the list storing lines
	lineList.clear();
	//clear the undo stack
	getUndo()->clear();
}

void StudentTextEditor::move(Dir dir) {
	list<string>::iterator it;

	if (lineList.size() == 0) {
		return;
	}

	it = lineList.begin();

	for (int i = 0; i < m_row; i++) {
		it++;
	}
	//move to different directions when different keys are pressed
	switch (dir) {
	case UP:
		if (m_row != 0) {
			if (m_col > (--it)->size()) {
				m_col = it->size();
			}
			m_row--;
		}
		break;

	case DOWN:
		if (m_row != lineList.size() - 1) {
			if (m_col > (++it)->size()) {
				m_col = it->size();
			}
			m_row++;
		}
		break;

	case LEFT:
		if (m_col != 0) {
			m_col--;
		}
		else if (m_col == 0) {
			if (m_row != 0) {
				m_row--;
				m_col = (--it)->size();
			}
		}
		break;

	case RIGHT:
		if (m_col != it->size()) {
			m_col++;
		}
		else {
			if (m_row != lineList.size() - 1) {
				m_row++;
				m_col = 0;
			}
		}
		break;

	case HOME:
		m_col = 0;
		break;

	case END:
		m_col = it->size() - 1;
		break;
	}
}

void StudentTextEditor::del() {
	list<string>::iterator it;

	if (lineList.size() == 0) {
		return;
	}

	it = lineList.begin();

	for (int i = 0; i < m_row; i++) {
		it++;
	}

	if (m_col == it->size() && m_col != 0) {
		return;
	}
	else if (m_col != 0 || it->size() != 0) {
		char c = (*it).at(m_col);
		it->erase(m_col, 1);
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, c);
	}
	else {
		string s = *(++it);
		it--;
		m_col = 0;
		(*it) += s;
		lineList.erase(++it);
		getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
	}
}

void StudentTextEditor::backspace() {
	list<string>::iterator it;

	if (lineList.size() == 0) {
		return;
	}

	it = lineList.begin();
	for (int i = 0; i < m_row; i++) {
		++it;
	}

	if (m_col != 0) {
		char c = (*it)[m_col - 1];
		it->erase(m_col - 1, 1);
		m_col--;
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, c);
	}
	else if (m_col == 0) {
		if (m_row != 0) {
			string s = *it;
			it--;
			(*it) += s;
			unsigned int size = it->size();
			lineList.erase(++it);
			getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
			m_row--;
			m_col = size;
		}
	}
}

void StudentTextEditor::insert(char ch) {
	list<string>::iterator it;

	if (lineList.size() != 0) {
		it = lineList.begin();
		for (int i = 0; i < m_row; i++) {
			it++;
		}
	}

	else {
		string blank = "";
		lineList.push_back(blank);
		it = lineList.begin();
	}

	string inserted = "";
	if (ch == '\t') {
		inserted = "    ";
		it->insert(m_col, inserted);
		m_col += 4;
	}
	else {
		inserted += ch;
		it->insert(m_col, inserted);
		m_col++;
	}
	getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
}

void StudentTextEditor::enter() {

	string blank = "";

	lineList.push_back(blank);

	list<string>::iterator it = lineList.begin();

	for (int i = 0; i < m_row; i++) {
		it++;
	}
	string s1 = it->substr(0, m_col);
	string s2 = it->substr(m_col);

	*it = s1;
	lineList.insert(++it, s2);

	getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
	m_col = 0;
	m_row++;

}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = m_row;
	col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	if (startRow < 0 || numRows < 0 || startRow > lineList.size()) {
		return -1;
	}
	vector<string>::const_iterator it1;
	list<string>::const_iterator it2 = lineList.begin();
	for (it1 = lines.begin(); it1 != lines.end();) {
		lines.erase(it1);
	}

	int i = 0;

	while (i != startRow + numRows && it2 != lineList.end()) {
		if (i >= startRow) {
			lines.push_back(*it2);
		}
		i++;
		it2++;
	}

	int result = lines.size();
	return result;
}

void StudentTextEditor::undo() {
	if (lineList.size() == 0) {
		return;
	}

	list<string>::iterator it = lineList.begin();
	int count = 0;
	string txt = "";
	Undo::Action Act;

	Act = getUndo()->get(m_row, m_col, count, txt);

	for (int i = 0; i < m_row; i++) {
		it++;
	}

	string temp = "";

	switch (Act) {
	case::Undo::Action::DELETE:
		for (int i = 0; i < count; i++) {
			(*it).erase(m_col - 1, 1);
			if (m_col >= 1) {
				m_col--;
			}
		}
		break;///
	case Undo::Action::INSERT:
		(*it).insert(m_col, txt);
		break;
	case Undo::Action::SPLIT:
		temp = (*it).substr(m_col);
		(*it).erase(m_col);
		if (m_row == lineList.size() - 1) {
			lineList.push_back("" + temp);
		}
		else {
			lineList.insert(++it, temp);
		}
		break;///
	case Undo::Action::JOIN:
		temp = (*(++it));
		(*(--it)).insert(m_col, temp);
		lineList.erase(++it);
		break;///
	case Undo::ERROR:
	default:
		return;
	}
}
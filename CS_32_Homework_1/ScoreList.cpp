#include "ScoreList.h"
#include "Sequence.h"
#include <iostream>
#include <limits>
#include <stdlib.h>

using namespace std;

ScoreList::ScoreList() {

}

bool ScoreList::add (unsigned long score){
	if (score > 100 || m_list.size() >= DEFAULT_MAX_ITEMS) {
		return false;
	}

	m_list.insert(score);
	return true;
}

bool ScoreList::remove(unsigned long score) {
	int pos = m_list.find(score);
	bool removed = false;

	if (pos == -1) {
		return removed;
	}

	removed = m_list.erase(pos);
	return removed;
}

int ScoreList::size() const {
	return m_list.size();
}

unsigned long ScoreList::minimum() const {
	if (m_list.empty()) {
		return NO_SCORE;
	}
	unsigned long getter = 0;
	m_list.get(0, getter);
	return getter;
}

unsigned long ScoreList::maximum() const {
	if (m_list.empty()) {
		return NO_SCORE;
	}
	unsigned long getter = 0;
	m_list.get((size()-1), getter);
	return getter;
}
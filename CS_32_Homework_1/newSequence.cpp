#include "newSequence.h"

#include <iostream>

using namespace std;

Sequence::Sequence() {
	m_size = 0;
	m_maxItem = DEFAULT_MAX_ITEMS;
	m_sequence = new ItemType[DEFAULT_MAX_ITEMS];
}

Sequence::Sequence(const int item) 
	:m_size(0), m_maxItem(item)
{
	m_sequence = new ItemType[m_maxItem];
	if (m_maxItem < 0) {
		exit(1);
	}
}

Sequence::Sequence(const Sequence& s1) {
	m_size = s1.m_size;
	m_maxItem = s1.m_maxItem;
	m_sequence = new ItemType[m_maxItem];
	for (int i = 0; i < m_size; i++) {
		m_sequence[i] = s1.m_sequence[i];
	}
}

Sequence::~Sequence() {
	delete []m_sequence;
}

Sequence& Sequence::operator=(const Sequence& opr) {
	delete []m_sequence;
	m_size = opr.size();
	m_maxItem= opr.m_maxItem;
	m_sequence = new ItemType[m_maxItem];
	for (int i = 0; i < m_maxItem; i++) {
		m_sequence[i] = opr.m_sequence[i];
	}
	return *this;
}

bool Sequence::empty() const {

	return (m_size == 0);
}

int Sequence::size() const {

	return m_size;
}

int Sequence::insert(int pos, const ItemType& value) {

	bool insertable = (0 <= pos && pos <= size() && size() < m_maxItem);

	if (!(insertable)) {
		return -1;
	}

	for (int i = size(); i > pos; i--) {
		m_sequence[i] = m_sequence[i - 1];
	}
	m_sequence[pos] = value;
	m_size++;
	return pos;
}

int Sequence::insert(const ItemType& value) {

	int pos = 0;

	if (m_size >= m_maxItem) {
		return -1;
	}

	for (int i = 0; i < size(); i++) {
		if (value <= m_sequence[i]) {
			pos = i;
			insert(pos, value);

			return pos;
		}
	}

	pos = size();
	insert(pos, value);
	return pos;
}

bool Sequence::erase(int pos) {

	bool erasable = (0 <= pos && pos < size());

	if (!(erasable)) {
		return false;
	}

	for (int i = pos; i < size(); i++) {
		m_sequence[i] = m_sequence[i + 1];
	}
	m_size--;

	return true;
}

int Sequence::remove(const ItemType& value) {
	int numRemoved = 0;

	for (int i = 0; i < size(); i++) {
		if (m_sequence[i] == value) {
			erase(i);
			numRemoved++;
			i--;
		}
	}

	return numRemoved;
}

bool Sequence::get(int pos, ItemType& value) const {

	bool gettable = (0 <= pos && pos < size());

	if (!(gettable)) {
		return false;
	}

	value = m_sequence[pos];

	return true;
}

bool Sequence::set(int pos, const ItemType& value) {

	bool settable = (0 <= pos && pos < size());

	if (!(settable)) {
		return false;
	}

	m_sequence[pos] = value;

	return true;
}

int Sequence::find(const ItemType& value) const {
	int p = -1;

	for (int i = 0; i < size(); i++) {
		if (m_sequence[i] == value) {
			p = i;
			return p;
		}
	}

	return p;
}

void Sequence::swap(Sequence& other) {
	Sequence temp = *this;
	*this = other;
	other = temp;
}
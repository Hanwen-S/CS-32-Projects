#ifndef COORD_H
#define COORD_H

class Coord
{
public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r () const { return m_row; }
    int c () const { return m_col; }

private:
    int m_row;
    int m_col;
};

#endif // !COORD_H
#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
	int r() const { return m_row; }
	int c() const { return m_col; }

private:
	int m_row;
	int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	stack<Coord> CoordStack;
	Coord start(sr, sc);
	Coord end(er, ec);
	CoordStack.push(start);
	maze[sr][sc] = '#';

	while (!CoordStack.empty()) {
		Coord curLoc = CoordStack.top();
		/*cout << CoordStack.top().r() << CoordStack.top().c() << endl;*/
		CoordStack.pop();
		if (curLoc.r() == end.r() && curLoc.c() == end.c()) {
			return true;
		}

		else {
			if (maze[curLoc.r() - 1][curLoc.c()] == '.') { //Check North
				CoordStack.push(Coord((curLoc.r() - 1), curLoc.c()));
				maze[curLoc.r() - 1][curLoc.c()] = '#';
			}

			if (maze[curLoc.r()][curLoc.c() + 1] == '.') { //Check East
				CoordStack.push(Coord(curLoc.r(), (curLoc.c() + 1)));
				maze[curLoc.r()][curLoc.c() + 1] = '#';
			}

			if (maze[curLoc.r() + 1][curLoc.c()] == '.') { //Check South
				CoordStack.push(Coord((curLoc.r() + 1), curLoc.c()));
				maze[curLoc.r() + 1][curLoc.c()] = '#';
			}

			if (maze[curLoc.r()][curLoc.c() - 1] == '.') { //Check West
				CoordStack.push(Coord((curLoc.r()), (curLoc.c() - 1)));
				maze[curLoc.r()][curLoc.c() - 1] = '#';
			}
		}
	}

	return false;
}

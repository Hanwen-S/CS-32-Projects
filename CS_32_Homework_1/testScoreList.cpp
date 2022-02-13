#include <iostream>
#include <string>
#include <cassert>
#include <stdlib.h>
#include <limits>
#include "Sequence.h"
#include "ScoreList.h"

using namespace std;

int main()
{
    ScoreList a;
    assert(a.add(60) == true);
    assert(a.remove(60) == true);
    assert(a.size() == 0);
    a.add(50);
    a.add(40);
    a.add(60);
    a.add(70);
    assert(a.minimum() == 40);
    assert(a.maximum() == 70);
    a.add(30);
    assert(a.minimum() == 30);
    a.remove(30);
    assert(a.minimum() == 40);
    cout << "all tests passed!" << endl;

	return 0;
}
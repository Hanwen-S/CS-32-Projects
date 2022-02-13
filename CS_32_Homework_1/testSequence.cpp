#include "Sequence.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
    Sequence a;

    ItemType v1 = 4;
    ItemType v2 = 5;
    ItemType v3 = 3;

    assert(a.empty() == true);
    assert(a.size() == 0);

    assert(a.insert(v1) == 0);
    assert(a.insert(v2) == 1);
    assert(a.insert(v3) == 0);
    assert(a.insert(6, v1) == -1);
    assert(a.size() == 3);

    assert(a.insert(v3) == 0);
    assert(a.insert(v3) == 0);

    assert(a.size() == 5);
    assert(a.erase(5) == false);
    assert(a.erase(4) == true);
    assert(a.remove(v3) == 3);
    assert(a.size() == 1);

    for (int k = 0; k < 149; k++) {
        assert(a.insert(v1) == 0);
    }
    assert(a.insert(v1) == -1);

    Sequence b;
    b.swap(a);
    assert(b.size() == 150);
    assert(a.size() == 0);
    assert(b.find(v1) == 0);
    assert(a.find(v1) == -1);
    ItemType value;
    assert(b.get(100, value) == true);
    assert(value == v1);
    assert(a.get(0, value) == false);
}


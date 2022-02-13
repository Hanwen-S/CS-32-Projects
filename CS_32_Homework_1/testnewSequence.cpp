#include "newSequence.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
    Sequence a(1000);
    Sequence b(5);
    Sequence c;
    Sequence d(7);
    Sequence e(9);
    ItemType v1 = 4;
    ItemType v2 = 5;
    ItemType v3 = 3;

    assert(a.empty() == true);
    assert(b.size() == 0);

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

    for (int k = 0; k < 5; k++) {
        assert(b.insert(v1) == 0);
    }
    assert(b.insert(v1) == -1);
    a.swap(b);
    assert(a.insert(v1) == -1 && b.insert(v1) != -1);
    d.swap(e);
    for (int i = 0; i < 9; i++) {
        assert(d.insert(v1) != -1);
    }
    for (int i = 0; i < 7; i++) {
        assert(e.insert(v1) != -1);
    }
    assert(e.insert(v1) == -1);
}


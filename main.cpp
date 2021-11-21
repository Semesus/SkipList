#include <cassert>
#include <iostream>

#include "SkipList.h"

using namespace std;

void test1() {
    // setting seed for easy testing
    srand(100000);
    cout << boolalpha;
    // test ctor
    SkipList skp(10, 90);
    cout << skp << endl;
    // test add()
    skp.add(6);
    assert(skp.contains(6));
    cout << skp << endl;
    skp.add(3);
    assert(skp.contains(3));
    cout << skp << endl;
    skp.add(10);
    assert(skp.contains(10));
    // test add() no duplicates
    skp.add(3);
    // test add() negative
    skp.add(-134);
    skp.add(-4);
    cout << skp << endl;
    // test add() zero
    skp.add(0);
    // test contains()
    cout << "6 ? " << skp.contains(6) << endl;
    cout << skp << endl;
    // test remove()
    skp.remove(3);
    assert(!(skp.contains(3)));
    cout << skp << endl;
    skp.remove(10);
    // test remove() error msg
    skp.remove(10);
    assert(!(skp.contains(10)));
    assert(!(skp.contains(42)));
    cout << skp << endl;
    cout << "-134 ? " << skp.contains(-134) << endl;
    cout << "-4 ? " << skp.contains(-4) << endl;
    cout << "0 ? " << skp.contains(0) << endl;
    cout << "3 ? " << skp.contains(3) << endl;
    cout << "6 ? " << skp.contains(6) << endl;
    cout << "10 ? " << skp.contains(10) << endl;
    cout << "Test 1 Successfully Completed!" << endl << endl;
}

void test2() {
    // setting seed for easy testing
    srand(10000);
    SkipList skp(6, 80);
    cout << boolalpha;
    skp.add(3);
    skp.add(5);
    cout << skp << endl;
    cout << "7 ? " << skp.contains(7) << endl;
    cout << "3 ? " << skp.contains(3) << endl;
    cout << "-100 ? " << skp.contains(-100) << endl;
    cout << "100 ? " << skp.contains(100) << endl;
    bool result = skp.remove(5);
    assert(result);
    result = skp.remove(5);
    assert(!result);
    cout << skp << endl;
    cout << "Test 2 Successfully Completed!" << endl << endl;
}

void test3() {
    // test p = 50, max = 5, n = 20
    SkipList skp(5, 50);
    for(int i = 0; i < 20; i++) {
        int num = rand() % 100;
        skp.add(num);
        cout << "After adding " << num << endl;
        cout << skp << endl;
    }
    cout << "Test 3 Successfully Completed!" << endl;
}

int main() {
    test1();
    test2();
    test3();
    cout << "Done." << endl;
    return 0;
}
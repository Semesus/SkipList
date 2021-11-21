#include <cassert>
#include <iostream>

#include "skiplist.h"

using namespace std;

void test() {
    // setting seed for easy testing
    srand(10000);
    cout << boolalpha;
    SkipList skp(6, 90);
    cout << skp << endl;
    skp.add(6);
    cout << skp << endl;
    skp.add(3);
    cout << skp << endl;
    skp.add(10);
    skp.add(3);
    //cout << "6 ? " << skp.contains(6) << endl;
    cout << skp << endl;
    skp.remove(3);
    cout << skp << endl;
    //cout << "3 ? " << skp.contains(3) << endl;
    //cout << "6 ? " << skp.contains(6) << endl;
    bool result;
}

void test1() {
  // setting seed for easy testing
  srand(10000);
  SkipList skp(5, 80);
  cout << boolalpha;
  skp.add(3);
  skp.add(5);
  //skp.add(vector<int>{1, 7, 9});
  // cannot add existing
  //assert(!skp.add(vector<int>{1, 7}));
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
}

int main() {
    test();
    //test1();

  cout << "Done." << endl;
  return 0;
}
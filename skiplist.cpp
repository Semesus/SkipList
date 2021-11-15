
//modified by mashhadi on 14th feb to add an alternative op<<

#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>

#include "skiplist.h"

using namespace std;

ostream &operator<<(ostream &out, const SkipList &skip) {
  for (int i = skip.maxLevel_ - 1; i >= 0; i--) {
    out << "Level: " + to_string(i) + " -- ";
    SNode* curr = skip.frontGuards_[i];
    while(curr != nullptr) {
      out<< to_string(curr->value_) + ", ";
      curr = curr->forward_;
    }
    out << "\n";
  }
  return out;
}

/*


ostream &operator<<(ostream &out, const SkipList &skip) {
  for (int d = skip.maxLevel - 1; d >= 0; d--) {
    out << d << ": ";
    auto *curr = skip.head->forward[d];
    if (curr != skip.tail) {
      out << curr->value;
      curr = curr->forward[d];
    }
    while (curr != nullptr && curr != skip.tail) {
      out << "-->" << curr->value;
      curr = curr->forward[d];
    }
    out << endl;
  }
  return out;
}
*/
SNode::SNode(int value) : value_{value}, forward_{nullptr}, backward_{nullptr},
    up_{nullptr}, down_{nullptr} {}

// how many forward/backward pointers it has
//int SNode::height() const { return 0; }

// increase the number of forward/backward pointers it has
//void SNode::increaseHeight() {}

SkipList::SkipList(int maxLevel, int probability)
    : maxLevel_{maxLevel}, probability_{probability} {
    assert(maxLevel > 0 && probability >= 0 && probability < 100);
    // create front/rear guards to specified depth
    frontGuards_ = new SNode* [maxLevel_];
    rearGuards_ = new SNode* [maxLevel_];
    // dummy head/tail nodes negate special cases
    auto* front = new SNode(INT_MIN);
    auto* back = new SNode(INT_MAX);
    // link front node to back node
    front->forward_ = back;
    // link back node to front
    back->backward_ = front;
    // link guards to dummy nodes
    frontGuards_[0] = front;
    rearGuards_[0] = back;
}

bool SkipList::shouldInsertAtHigher() const {
  return rand() % 100 < probability_;
}

//bool SkipList::add(const vector<int> &values) { return true; }

bool SkipList::add(int value) {
    return true;
}

SkipList::~SkipList() {
  // need to delete individual nodes
}

bool SkipList::remove(int data) { return true; }



// Given a SNode, place it before the given NextNode
void SkipList::addBefore(SNode *newNode, SNode *nextNode) {
  // Link next to node in front
  newNode->forward_ = nextNode;
  // Link prev to node behind
  newNode->backward_ = nextNode->forward_;
  // Link node in back to new node
  nextNode->backward_->forward_ = newNode;
  // Link node in front to new node
  nextNode->backward_ = newNode;
}

// get the node that would be before this data
// at level-0
//vector<SNode *> SkipList::getBeforeNodes(int data) const {
//  vector<SNode *> v(maxLevel);
//  return v;
//}

// Checks to see whether or not a data value exists in the list
// Returns the NODE if the value exists in the SkipList.
// Returns nullptr otherwise

SNode* SkipList::containsSNode(int data) const { return nullptr; }

// Checks to see whether or not a data value exists in the list
// Returns true if the value exists in the SkipList.
// Returns false otherwise

bool SkipList::contains(int data) const {
    return true;
}



//private methods to ease linking
//void SkipList::connect2AtLevel(SNode *a, SNode *b, int level) {}

//void SkipList::connect3AtLevel(SNode *a, SNode *b, SNode *c, int level) {}

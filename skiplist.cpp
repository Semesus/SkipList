
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
      curr = curr->next_;
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
SNode::SNode(int value) : value_{value}, next_{nullptr}, prev_{nullptr},
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
    front->next_ = back;
    // link back node to front
    back->prev_ = front;
    // link guards to dummy nodes
    frontGuards_[0] = front;
    rearGuards_[0] = back;
}

bool SkipList::shouldInsertAtHigher() const {
  return rand() % 100 < probability_;
}

//bool SkipList::add(const vector<int> &values) { return true; }

bool SkipList::add(int value) {
    // pointer to beginning of list
    SNode* curr = frontGuards_[0]->next_;
    // create new node to insert
    auto* newNode = new SNode(value);
    // iterate thru list until greater value found
    while(curr->value_ < value) {
        curr = curr->next_;
    }
    // Add node into list
    addBefore(newNode, curr);
    return true;
}

SkipList::~SkipList() {
  // need to delete individual nodes
}

bool SkipList::remove(int data) {
    SNode* curr = frontGuards_[0];
    while(curr->value_ != data) {
        if(curr->next_->value_ == INT_MAX) {
            break;
        } else if(curr->next_->value_ == data) {
            SNode* temp = curr->next_;
            curr->next_->next_->prev_ = curr;
            curr->next_ = curr->next_->next_;
            delete temp;
            break;
        }
        curr = curr->next_;
    }
    return true;
}



// Given a SNode, place it before the given NextNode
void SkipList::addBefore(SNode* newNode, SNode* nextNode) {
    assert(newNode != nullptr && nextNode != nullptr && newNode->value_ < nextNode->value_);
    // Link next to node in front
    newNode->next_ = nextNode;
    // Link prev to node behind
    newNode->prev_ = nextNode->prev_;
    // Link node in back to new node
    nextNode->prev_->next_ = newNode;
    // Link node in front to new node
    nextNode->prev_ = newNode;
    assert(newNode->next_ == nextNode && nextNode->prev_ == newNode);
    assert(newNode->prev_ != nullptr && newNode->prev_->value_ < newNode->value_);
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

//SNode* SkipList::containsSNode(int data) const { return nullptr; }

// Checks to see whether or not a data value exists in the list
// Returns true if the value exists in the SkipList.
// Returns false otherwise

bool SkipList::contains(int data) {
    SNode* curr = frontGuards_[maxLevel_ - 1];
    while(curr != nullptr) {
        if(curr->next_->value_ < data) {
            curr = curr->next_;
        } else if(curr->next_->value_ == data) {
            return true;
        } // else if(curr->next_->value_ > data {
                //curr = curr->down_;
        // }
        else {
            // redundant?
            return false;
        }
    }
    return false;
}



//private methods to ease linking
//void SkipList::connect2AtLevel(SNode *a, SNode *b, int level) {}

//void SkipList::connect3AtLevel(SNode *a, SNode *b, SNode *c, int level) {}

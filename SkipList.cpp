
//modified by mashhadi on 14th feb to add an alternative op<<

#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>

#include "SkipList.h"

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

SNode::SNode(int value) : value_{value}, next_{nullptr}, prev_{nullptr},
    up_{nullptr}, down_{nullptr} {}

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
    // create levels up to maxLevel
    for(int i = 1; i < maxLevel; i++) {
        front = new SNode(INT_MIN);
        back = new SNode(INT_MAX);
        front->next_ = back;
        back->prev_ = front;
        frontGuards_[i] = front;
        rearGuards_[i] = back;
        // link up/down pointers
        front->down_ = frontGuards_[i - 1];
        frontGuards_[i - 1]->up_ = front;
        back->down_ = rearGuards_[i - 1];
        rearGuards_[i - 1]->up_ = back;
    }
}

SkipList::~SkipList() {
    // need to delete individual nodes
    for(int i = maxLevel_ - 1; i >= 0; i--) {
        SNode* curr = frontGuards_[i];
        while(curr->next_ != nullptr) {
            SNode* prev = curr;
            curr = curr->next_;
            delete prev;
        }
        delete curr;
    }
    delete frontGuards_;
    delete rearGuards_;
}

bool SkipList::shouldInsertAtHigher() const {
    return rand() % 100 < probability_;
}

bool SkipList::add(int value) {
    // check if value already in list
    if(contains(value)) {
        return false;
    }
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
    // check if to be added to higher level
    bool levelUp = shouldInsertAtHigher();
    int currLevel = 1;
    while(levelUp && (currLevel < maxLevel_ - 1)) {
        // continue checking levelUp
        curr = frontGuards_[currLevel]->next_;
        while(curr->value_ < value) {
            curr = curr->next_;
        }
        auto* levelUpNode = new SNode(value);
        addBefore(levelUpNode, curr);
        // link up/down pointers
        newNode->up_ = levelUpNode;
        levelUpNode->down_ = newNode;
        currLevel++;
        levelUp = shouldInsertAtHigher();
    }
    return true;
}

bool SkipList::remove(int data) {
    if(!(contains(data))) {
        return false;
    }
    for(int i = maxLevel_ - 1; i >= 0; i--) {
        SNode* curr = frontGuards_[i];
        if(curr->next_->value_ == INT_MAX) {
            i--;
            curr = frontGuards_[i];
        }
        while(curr->value_ != data) {
            if(curr->next_->value_ == INT_MAX) {
                break;
            } else if(curr->next_->value_ == data) {
                SNode* temp = curr->next_;
                curr->next_ = curr->next_->next_;
                delete temp;
                break;
            }
            curr = curr->next_;
        }
    }
    return true;
}

// Checks to see whether or not a data value exists in the list
// Returns true if the value exists in the SkipList.
// Returns false otherwise
bool SkipList::contains(int data) const {
    SNode* curr = frontGuards_[maxLevel_ - 1];
    while(curr != nullptr) {
        if(curr->next_->value_ < data) {
            curr = curr->next_;
        } else if(curr->next_->value_ == data) {
            return true;
        } else if(curr->next_->value_ > data) {
            curr = curr->down_;
        }
        else {
            // redundant?
            return false;
        }
    }
    return false;
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


/************************************************************************
 * SkipList.cpp
 * Devin Clarke
 * CSS 342
 * Created 11/10/21
 * C++
 *
 * SkipList data structure to store integers. SkipList with a depth
 * of 1 is similar to a doubly-linked list. Each item has a p percent
 * chance of being at the next level up. For our implementation p = 50%
 * All elements are inserted at the lowest Depth (1)
 * 50% of the elements inserted in Depth = 2
 * 50% of 50%, or 25% of the elements inserted in Depth = 3
 * and so on
 *
 * If a Skip List has only 1 level, such as p = 0%
 * Insert O(n), Search O(n)
 * For Depth > 1
 * Remove O(log n), Search O(log n)
 * Modifying p allows us to trade off search speed and storage cost
 * *********************************************************************/

#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>

#include "SkipList.h"

using namespace std;

/**
 * Overloaded ostream operator<< prints each level of each node
 * value in a SkipList
 * @param: out: ostream object
 * @param: skip: SkipList object to display
 * @post: properly displayed SkipList nodes by level
 * @return: valid ostream reference
 * */
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

// Constructor creates node, assigns value and sets all pointers null
SNode::SNode(int value) : value_{value}, next_{nullptr}, prev_{nullptr},
    up_{nullptr}, down_{nullptr} {}

/**
 * SkipList Class Constructor
 * Creates empty SkipList, initializes depth to maxLevel
 * @pre: maxLevel > 0
 * @pre: 0 <= probability < 100
 * @param: maxLevel: Depth of SkipList
 * @param: probability: prob. of values added to next level
 * @post: empty SkipList of specified depth
 * */
SkipList::SkipList(int maxLevel, int probability)
    : maxLevel_{maxLevel}, probability_{probability} {
    assert(maxLevel > 0 && probability >= 0 && probability < 100);
    // create front/rear guards to specified depth
    frontGuards_ = new SNode* [maxLevel_];
    rearGuards_ = new SNode* [maxLevel_];
    // dummy head/tail nodes remove special cases
    auto* front = new SNode(INT_MIN);
    auto* back = new SNode(INT_MAX);
    // link front node->back node
    front->next_ = back;
    // link back node->front
    back->prev_ = front;
    // assign dummy nodes to guards
    frontGuards_[0] = front;
    rearGuards_[0] = back;
    // create remaining levels up to maxLevel
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

/**
 * SkipList Class Destructor
 * Begins at max level deleting all nodes down to
 * level 1, then deletes guard arrays
 * @pre: SkipList object
 * @post: all dynamically allocated memory
 * released back to system
 * */
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
    delete[] frontGuards_;
    delete[] rearGuards_;
}

/**
 * Determines if newly added node should be inserted
 * to higher level
 * @return: true if should insert higher, false otherwise
 * */
bool SkipList::shouldInsertAtHigher() const {
    return rand() % 100 < probability_;
}

/**
 * Inserts a node into SkipList in sorted order
 * No duplicates allowed
 * @pre: INT_MIN <= value <= INT_MAX
 * @param: value: integer value to insert
 * @post: node inserted with pointers properly linked
 * @return: true if added successfully, else false
 * */
bool SkipList::add(int value) {
    // check if value exists in list
    if(contains(value)) {
        cout << "Duplicates not allowed: " << value << endl;
        return false;
    }
    // pointer to list beginning
    SNode* curr = frontGuards_[0]->next_;
    // create new node to be inserted
    auto* newNode = new SNode(value);
    // iterate until greater value found
    while(curr->value_ < value) {
        curr = curr->next_;
    }
    // add node
    addBefore(newNode, curr);
    // check if needs to be added to higher level
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

/**
 * Removes all nodes containing given data value
 * @pre: non-empty SkipList
 * @param: data: value to remove
 * @post: No nodes with given data remain in SkipList,
 * pointers of residual nodes relinked properly
 * @return: true if successfully removed, else false
 * */
bool SkipList::remove(int data) {
    if(!(contains(data))) {
        cout << "Value not contained in SkipList: " << data << endl;
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

/**
 * Contains begins at max level, traversing list for
 * target
 * @param: data: value to find
 * @return true if value found, else false
 * */
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


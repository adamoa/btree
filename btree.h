#ifndef BTREE_H_
#define BTREE_H_

#include <string>

using namespace std;

class BTree {
private:
  BTree * p_parent;
  BTree * p_left;
  BTree * p_right;
  int d_factor;
  int d_value;


  BTree(BTree * parent, int value);
  void updateChild(BTree * original, BTree * replacement);
  void insertLeft(int value);
  void insertRight(int value);
  BTree * drillDown(int value);
  int adjustRight();
  int adjustLeft();
  void heightIncrease(BTree * child);
  void heightDecrease(BTree * child);
  void rotateRight();
  void rotateLeft();
  void remove(BTree * node);
  void printElements();

public:
  BTree();
  int getValue();
  void insert(int value);
  void remove(int value);
  BTree * lookup(int value);
  void print();
};

#endif

#ifndef BTREE_H_
#define BTREE_H_

#include <fstream>

using namespace std;

class BTree {
private:
  BTree * p_parent;
  BTree * p_left;
  BTree * p_right;
  int d_factor;
  int d_value;
  fstream * f_file;
  streampos p_fpos;


  BTree(BTree * parent, int value);
  BTree(BTree * parent, streampos fpos);
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
  void store();
  void retrieve();

public:
  BTree(const char * file);
  int getValue();
  void insert(int value);
  void remove(int value);
  BTree * lookup(int value);
  void print();
  void storeTree();
};

#endif

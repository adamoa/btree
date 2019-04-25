#ifndef BTREE_H_
#define BTREE_H_

#include <fstream>

using namespace std;

class BST {
private:
  BST * p_parent;
  BST * p_left;
  BST * p_right;
  int d_factor;
  int d_value;
  fstream * f_file;
  streampos p_fpos;


  BST(BST * parent, int value);
  BST(BST * parent, streampos fpos);
  void updateChild(BST * original, BST * replacement);
  void insertLeft(int value);
  void insertRight(int value);
  BST * drillDown(int value);
  int adjustRight();
  int adjustLeft();
  void heightIncrease(BST * child);
  void heightDecrease(BST * child);
  void rotateRight();
  void rotateLeft();
  void remove(BST * node);
  void printElements();
  void store();
  void retrieve();

public:
  BST(const char * file);
  int getValue();
  void insert(int value);
  void remove(int value);
  BST * lookup(int value);
  void print();
  void storeTree();
  void traverse();
};

#endif

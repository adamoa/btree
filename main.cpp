
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "btree.h"
using namespace std;

int main () {
  fstream file("test.out", ios_base::in | ios_base::out | ios_base::trunc);
  file.close();

  BTree *root = new BTree("test.out");
  for (int i=0; i<50; i++) {
    int value=rand()%1000;
    root->insert(value);
  }

  root->print();

  root->storeTree();

  return 0;
}

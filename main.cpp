
#include <iostream>
#include <stdlib.h>
#include "btree.h"
using namespace std;

int main () {
  BTree *root = new BTree();
  for (int i=0; i<50; i++)
    root->insert(rand()%1000);

  root->remove(123);
  root->remove(69);
  root->remove(135);
  root->remove(886);

  root->print();

  return 0;
}

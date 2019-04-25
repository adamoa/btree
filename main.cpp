
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "bst.h"
using namespace std;

int main () {
  fstream file("test.out", ios_base::in | ios_base::out | ios_base::trunc);
  file.close();

  BST *root = new BST("test.out");

  cout<<"// Input:\n";
  for (int i=0; i<50; i++) {
    int value=rand()%1000;
    cout<<"// "<<value<<"\n";
    root->insert(value);
  }

  cout<<"// Graph:\n";
  root->print();

  root->storeTree();

  cout<<"// Output:\n";
  root->traverse();

  return 0;
}

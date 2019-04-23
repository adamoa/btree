#include "btree.h"
#include <iostream>

using namespace std;

BTree::BTree() {
  d_value=0;
  d_factor=0;
  p_parent=0;
  p_left=0;
  p_right=0;
}

BTree::BTree(BTree * parent, int value) {
  d_value=value;
  d_factor=0;
  p_parent=parent;
  p_left=0;
  p_right=0;
}

void BTree::updateChild(BTree * original, BTree * replacement) {
  if (p_right==original) {

    p_right=replacement;
    if (replacement)
      replacement->p_parent=this;

  } else if (p_left==original) {

    p_left=replacement;
    if (replacement)
      replacement->p_parent=this;

  }
}

void BTree::rotateRight() {
  if (p_parent) {
    BTree * temporary=p_left;

    p_left=temporary->p_right;
    if (p_left)
      p_left->p_parent=this;

    p_parent->updateChild(this, temporary);

    temporary->p_right=this;
    p_parent=temporary;
  }
}

void BTree::rotateLeft() {
  if (p_parent) {
    BTree * temporary=this->p_right;

    this->p_right=temporary->p_left;
    if (p_right)
      p_right->p_parent=this;

    p_parent->updateChild(this, temporary);

    temporary->p_left=this;
    p_parent=temporary;
  }
}

int BTree::adjustRight() {
  if (p_right->d_factor==-1) {

    switch (p_right->p_left->d_factor) {
      case 0:
        d_factor=0;
        p_right->d_factor=0;
        break;
      case 1:
        d_factor=-1;
        p_right->d_factor=0;
        break;
      case -1:
        d_factor=0;
        p_right->d_factor=1;
        break;
    }

    p_right->p_left->d_factor=0;

    p_right->rotateRight();
    rotateLeft();

    return -1;

  } else if (p_right->d_factor==1) {
    d_factor=0;
    p_right->d_factor=0;
    rotateLeft();

    return -1;

  }  else if (p_right->d_factor==0) {
    d_factor=1;
    p_right->d_factor=-1;
    rotateLeft();

    return 0;
  }

  return 0;
}

int BTree::adjustLeft() {
  if (p_left->d_factor==1) {

    switch (p_left->p_right->d_factor) {
      case 0:
        d_factor=0;
        p_left->d_factor=0;
        break;
      case 1:
        d_factor=0;
        p_left->d_factor=-1;
        break;
      case -1:
        d_factor=1;
        p_left->d_factor=0;
        break;
    }

    p_left->p_right->d_factor=0;

    p_left->rotateLeft();
    rotateRight();

    return -1;

  } else if (p_left->d_factor==-1) {
    d_factor=0;
    p_left->d_factor=0;
    rotateRight();

    return -1;

  } else if (p_left->d_factor==0) {
    d_factor=-1;
    p_left->d_factor=1;
    rotateRight();

    return 0;
  }

  return 0;
}

void BTree::heightIncrease(BTree * child) {
  if (p_parent) {

    if (p_right==child) {

      if (d_factor==1) {
        adjustRight();
        return;

      } else {
        d_factor++;
        if (d_factor==1)
          p_parent->heightIncrease(this);
      }

    } else if (p_left==child) {
      if (d_factor==-1) {
        adjustLeft();
        return;

      } else {
        d_factor--;
        if (d_factor==-1)
          p_parent->heightIncrease(this);
      }
    }
  }
}

void BTree::heightDecrease(BTree * child) {
  if (p_parent) {

    if (p_right==child) {

      if (d_factor==-1) {

        if (adjustLeft()==-1) {
          p_parent->p_parent->heightDecrease(p_parent);
        }
        return;

      } else {
        d_factor--;
        if (d_factor==0)
          p_parent->heightDecrease(this);
      }

    } else if (p_left==child) {
      if (d_factor==1) {

        if (adjustRight()==-1) {
          p_parent->p_parent->heightDecrease(p_parent);
        }
        return;

      } else {
        d_factor++;
        if (d_factor==0)
          p_parent->heightDecrease(this);
      }
    }
  }
}

void BTree::insertLeft(int value) {
  if (p_left==0) {
    p_left=new BTree(this, value);
    heightIncrease(p_left);
  }
}

void BTree::insertRight(int value) {
  if (p_right==0) {
    p_right=new BTree(this, value);
    heightIncrease(p_right);
  }
}

void BTree::insert(int value) {
  if (p_parent==0 && p_right==0) {
    p_right=new BTree(this, value);
  } else if (p_parent==0) {
    BTree * parent = p_right->lookup(value);

    if (value > parent->getValue())
      parent->insertRight(value);
    else if (value < parent->getValue())
      parent->insertLeft(value);
  }

}

void BTree::remove(BTree * node) {
  if (node->p_right==0) {
    node->p_parent->heightDecrease(node);
    node->p_parent->updateChild(node, node->p_left);
    delete node;
    return;
  } else if (node->p_left==0) {
    node->p_parent->heightDecrease(node);
    node->p_parent->updateChild(node, node->p_right);
    delete node;
    return;
  } else {
    BTree * replacement=node->p_left->lookup(node->d_value);
    int temp=replacement->d_value;
    remove(replacement);
    node->d_value=temp;
  }
}

void BTree::remove(int value) {
  BTree * node = lookup(value);

  if (node)
    if (node->d_value==value)
      remove(node);
}

BTree * BTree::drillDown(int value) {
  if (value > d_value)
    return p_right;
  else if (value < d_value)
    return p_left;
  else
    return this;
}

BTree * BTree::lookup(int value) {
  BTree * parent, *child;
  if (p_parent==0 && p_right!=0)
    parent=this->p_right;
  else if (p_parent!=0)
    parent=this;
  else
    return 0;

  child=parent->drillDown(value);

  while (child) {
    if (child==parent)
      return child;
    else
      parent=child;
      child=parent->drillDown(value);
  }

  return parent;
}

int BTree::getValue() {
  return d_value;
}

void BTree::printElements() {
  if (p_parent==0 && p_right!=0) {
    p_right->printElements();
  } else {

    cout << d_value << " [ label = <<B>" << d_value << "</B><BR/>" << d_factor << ">]\n";
    cout << d_value << " -> ";
    if (p_left!=0) {
      cout<<p_left->getValue()<<"\n";
    } else {
      cout<<"l"<<d_value<<" [ arrowhead=none, style = dotted ]\n";
      cout<<"l"<<d_value<<" [ style = invisible ]\n";
    }

    cout<<d_value<<" -> ";
    if (p_right!=0) {
      cout<<p_right->getValue()<<"\n";
    } else {
      cout<<"r"<<d_value<<" [ arrowhead=none, style = dotted ]\n";
      cout<<"r"<<d_value<<" [ style = invisible ]\n";
    }

    if (p_left!=0) {
      p_left->printElements();
    }
    if (p_right!=0) {
      p_right->printElements();
    }

  }
}

void BTree::print() {
  cout << "digraph {\ngraph [ ordering = out ]\nnode [ shape = circle ]\n";
  this->printElements();
  cout << "}\n";
}

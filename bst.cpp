#include "bst.h"
#include <iostream>

using namespace std;

BST::BST(const char * file) {
  p_parent=0;
  p_fpos=0;

  f_file = new fstream(file, ios_base::in | ios_base::out | ios_base::binary);
  f_file->peek();

  if (f_file->eof()) {
    f_file->clear();
    d_value=0;
    d_factor=0;
    p_left=0;
    p_right=0;

    store();
  } else {
    retrieve();
  }
}

BST::BST(BST * parent, int value) {
  d_value=value;
  d_factor=0;
  p_parent=parent;
  p_left=0;
  p_right=0;

  f_file=parent->f_file;
  p_fpos=f_file->tellp();

  store();
}

BST::BST(BST * parent, streampos fpos) {
  p_parent=parent;
  p_fpos=fpos;
  f_file=parent->f_file;

  retrieve();
}

void BST::updateChild(BST * original, BST * replacement) {
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

void BST::rotateRight() {
  if (p_parent) {
    BST * temporary=p_left;

    p_left=temporary->p_right;
    if (p_left)
      p_left->p_parent=this;

    p_parent->updateChild(this, temporary);

    temporary->p_right=this;
    p_parent=temporary;
  }
}

void BST::rotateLeft() {
  if (p_parent) {
    BST * temporary=this->p_right;

    this->p_right=temporary->p_left;
    if (p_right)
      p_right->p_parent=this;

    p_parent->updateChild(this, temporary);

    temporary->p_left=this;
    p_parent=temporary;
  }
}

int BST::adjustRight() {
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

int BST::adjustLeft() {
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

void BST::heightIncrease(BST * child) {
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

void BST::heightDecrease(BST * child) {
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

void BST::insertLeft(int value) {
  if (p_left==0) {
    p_left=new BST(this, value);
    heightIncrease(p_left);
  }
}

void BST::insertRight(int value) {
  if (p_right==0) {
    p_right=new BST(this, value);
    heightIncrease(p_right);
  }
}

void BST::insert(int value) {
  if (p_parent==0 && p_right==0) {
    p_right=new BST(this, value);
  } else if (p_parent==0) {
    BST * parent = p_right->lookup(value);

    if (value > parent->getValue())
      parent->insertRight(value);
    else if (value < parent->getValue())
      parent->insertLeft(value);
  }

}

void BST::remove(BST * node) {
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
    BST * replacement=node->p_left->lookup(node->d_value);
    int temp=replacement->d_value;
    remove(replacement);
    node->d_value=temp;
  }
}

void BST::remove(int value) {
  BST * node = lookup(value);

  if (node)
    if (node->d_value==value)
      remove(node);
}

BST * BST::drillDown(int value) {
  if (value > d_value)
    return p_right;
  else if (value < d_value)
    return p_left;
  else
    return this;
}

BST * BST::lookup(int value) {
  BST * parent, *child;
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

int BST::getValue() {
  return d_value;
}

void BST::traverse() {
  if (p_parent==0) {
    if (p_right)
      p_right->traverse();
    else
      return;
  } else {
    if (p_left)
      p_left->traverse();
    cout<<"// "<<d_value<<"\n";
    if (p_right)
      p_right->traverse();
  }


}

void BST::printElements() {
  if (p_parent==0 && p_right!=0) {
    p_right->printElements();
  } else {

    cout << d_value << " [ label = <<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\"><TR><TD PORT=\"L\"></TD><TD PORT=\"C\"><B>" << d_value << "</B><BR/>" << d_factor << "</TD><TD PORT=\"R\"></TD></TR></TABLE>>]\n";

    if (p_left!=0) {
      cout << d_value << ":L -> ";
      cout<<p_left->getValue()<<":C\n";
    }


    if (p_right!=0) {
      cout<<d_value<<":R -> ";
      cout<<p_right->getValue()<<":C\n";
    }

    if (p_left!=0) {
      p_left->printElements();
    }
    if (p_right!=0) {
      p_right->printElements();
    }

  }
}

void BST::store() {
  char data[3*sizeof(int)+1];
  int *value=reinterpret_cast<int *>(data);
  char *factor=reinterpret_cast<char *>(value+1);
  int *left=reinterpret_cast<int *>(factor+1);
  int *right=reinterpret_cast<int *>(left+1);


  *value=d_value;
  *factor=d_factor;

  if (p_left)
    *left=p_left->p_fpos;
  else
    *left=0;

  if (p_right)
    *right=p_right->p_fpos;
  else
    *right=0;

  f_file->seekp(p_fpos);
  f_file->write(data, sizeof(data));
}

void BST::retrieve() {
  char data[3*sizeof(int)+1];
  int *value=reinterpret_cast<int *>(data);
  char *factor=reinterpret_cast<char *>(value+1);
  int *left=reinterpret_cast<int *>(factor+1);
  int *right=reinterpret_cast<int *>(left+1);

  f_file->seekg(p_fpos);
  f_file->read(data, sizeof(data));

  d_value=*value;
  d_factor=*factor;

  if (*left!=0) {
    p_left=new BST(this, streampos(*left));
  }

  if (*right!=0) {
    p_right=new BST(this, streampos(*right));
  }
}

void BST::storeTree() {
  store();

  if (p_left!=0) {
    p_left->storeTree();
  }
  if (p_right!=0) {
    p_right->storeTree();
  }
}

void BST::print() {
  cout << "digraph {\ngraph [ ordering = out ]\nnode [ shape = none ]\n";
  this->printElements();
  cout << "}\n";
}

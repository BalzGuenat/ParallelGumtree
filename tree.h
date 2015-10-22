#ifndef TREE_H
#define TREE_H

#include <string>

using namespace std;

class Tree
{
public:
  Tree(int type, string label);
  string label();
private:
  int _type;
  string _label;
};

inline string Tree::label() { return _label; }

#endif // TREE_H

#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

using namespace std;

class FileParser;

class Tree
{
  friend class FileParser;
public:
  Tree(int type, string label, int lineNumber);
  string label() const;
  vector<Tree*> children() const;
  string toString() const;
  string subTreeToString() const;
private:
  vector<string> subTreeToStringVector() const;
  int _type;
  int _lineNumber;
  string _label;
  Tree* _parent;
  vector<Tree*> _children;
};

inline string Tree::label() const { return _label; }
inline vector<Tree*> Tree::children() const {
  return vector<Tree*>(_children);
}

#endif // TREE_H

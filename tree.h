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
  ~Tree();

  int type() const;
  int lineNumber() const;
  string label() const;

  Tree* parent() const;
  vector<Tree*> children() const;

  string toString() const;
  string subTreeToString() const;
private:
  vector<string> subTreeToStringVector() const;
  const int _type;
  const int _lineNumber;
  const string _label;
  Tree* _parent;
  vector<Tree*> _children;
};

inline int Tree::type() const { return _type; }
inline int Tree::lineNumber() const { return _lineNumber; }
inline string Tree::label() const { return _label; }
inline vector<Tree*> Tree::children() const {
  return vector<Tree*>(_children);
}

#endif // TREE_H

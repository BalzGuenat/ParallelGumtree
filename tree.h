#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

using namespace std;

class FileParser;
class TreeCreator;

class Tree
{
  friend class FileParser;
  friend class TreeCreator;
public:
  Tree(int type, string label, int lineNumber);
  ~Tree();

  int type() const;
  int lineNumber() const;
  string label() const;
  unsigned size() const;
  int id() const;
  void set_id(int id);

  bool isRoot() const;
  bool isLeaf() const;
  bool isClone(Tree* other) const;
  bool isMatched() const;
  int childPosition(const Tree* child) const;
  unsigned height() const;

  Tree* parent() const;
  vector<Tree*> children() const;

  //To be implemented ?
  vector<Tree*> get_trees();
  void set_matched(bool isMatched = true);

  string toString() const;
  string subTreeToString() const;
private:
  vector<string> subTreeToStringVector() const;
  void get_trees(Tree* tree, vector<Tree*>& trees);
  const int _type;
  const int _lineNumber;
  const string _label;
  Tree* _parent;
  vector<Tree*> _children;
  bool _isMatched = false;
  int _id;
  unsigned _height;
};

inline int Tree::type() const { return _type; }
inline int Tree::lineNumber() const { return _lineNumber; }
inline string Tree::label() const { return _label; }
inline bool Tree::isRoot() const { return !_parent; }
inline Tree* Tree::parent() const { return _parent; }
inline vector<Tree*> Tree::children() const {
  return vector<Tree*>(_children);
}
inline bool Tree::isLeaf() const { return _children.empty(); }
inline bool Tree::isMatched() const { return _isMatched; }
inline void Tree::set_matched(bool isMatched) { _isMatched = isMatched; }
inline int Tree::id() const { return _id; }
inline void Tree::set_id(int id) { _id = id; }
inline unsigned Tree::height() const { return _height; }

#endif // TREE_H

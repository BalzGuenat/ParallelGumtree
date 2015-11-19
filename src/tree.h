#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <stack>

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

  struct PostOrderStruct {
	  private:
		  Tree* _root;
	  public:
		  PostOrderStruct(Tree* root);
		  class PostOrderIterator : public iterator<input_iterator_tag, Tree*> {
			  private:
				  void push(Tree* tree);
				  stack<pair<Tree*, vector<Tree*>::const_iterator>> _stack;
			  public:
				  PostOrderIterator(Tree* tree);
				  PostOrderIterator& operator++();
				  bool operator==(const PostOrderIterator& other);
				  bool operator!=(const PostOrderIterator& other);
				  Tree* operator*() const;
		  };
		  static PostOrderIterator END;
		  PostOrderIterator begin();
		  PostOrderIterator end();
  };

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

  Tree* clone();

  Tree* parent() const;
  const vector<Tree*>& children() const;

  vector<Tree*> get_trees();
  PostOrderStruct postOrder();
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
inline const vector<Tree*>& Tree::children() const {
  return _children;
}
inline bool Tree::isLeaf() const { return _children.empty(); }
inline bool Tree::isMatched() const { return _isMatched; }
inline void Tree::set_matched(bool isMatched) { _isMatched = isMatched; }
inline int Tree::id() const { return _id; }
inline void Tree::set_id(int id) { _id = id; }
inline unsigned Tree::height() const { return _height; }
inline Tree::PostOrderStruct Tree::postOrder() { return PostOrderStruct(this); }

#endif // TREE_H

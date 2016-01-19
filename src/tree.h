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
				  Tree* _root;
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

  struct PreOrderStruct {
	  private:
		  Tree* _root;
	  public:
		  PreOrderStruct(Tree* root);
		  class PreOrderIterator : public iterator<input_iterator_tag, Tree*> {
			  private:
				  stack<pair<Tree*,vector<Tree*>::const_iterator>> _stack;
			  public:
				  PreOrderIterator(Tree* tree);
				  PreOrderIterator& operator++();
				  bool operator==(const PreOrderIterator& other);
				  bool operator!=(const PreOrderIterator& other);
				  Tree* operator*() const;
		  };
		  static PreOrderIterator END;
		  PreOrderIterator begin();
		  PreOrderIterator end();
  };

  int type() const;
  int lineNumber() const;
  string label() const;
  unsigned size() const;
  int id() const;
  void set_id(int id);
  unsigned hash() const;

  bool isRoot() const;
  bool isLeaf() const;
  bool isClone(Tree* other) const;
  bool isMatched() const;
  int childPosition(const Tree* child) const;
  unsigned height() const;
  unsigned depth() const;
  void set_size(unsigned size);
  void set_height(unsigned height);
  void set_depth(unsigned depth);

  void refresh();
  void compute_size();
  void compute_depth();
  void compute_height();
  void compute_hash();

  Tree* clone();

  Tree* parent() const;
  const vector<Tree*>& children() const;
  vector<Tree*> descendants();

  vector<Tree*> get_trees();
  PreOrderStruct preOrder();
  PostOrderStruct postOrder();
  void set_matched(bool isMatched = true);

  string toString() const;
  string subTreeToString() const;

private:

  static bool is_clone_parallel(const Tree* a, const Tree* b);
  vector<string> subTreeToStringVector() const;
  void get_trees(Tree* tree, vector<Tree*>& trees);
  const int _type;
  const int _lineNumber;
  const string _label;
  Tree* _parent;
  vector<Tree*> _children;
  bool _isMatched = false;
  int _id;
  unsigned _size;
  unsigned _height;
  unsigned _depth;
  unsigned _hash;
};

inline int Tree::type() const { return _type; }
inline int Tree::lineNumber() const { return _lineNumber; }
inline string Tree::label() const { return _label; }
inline bool Tree::isRoot() const { return !_parent; }
inline Tree* Tree::parent() const { return _parent; }
inline const vector<Tree*>& Tree::children() const {
  return _children;
}
inline unsigned Tree::hash() const { return _hash; }
inline bool Tree::isLeaf() const { return _children.empty(); }
inline bool Tree::isMatched() const { return _isMatched; }
inline void Tree::set_matched(bool isMatched) { _isMatched = isMatched; }
inline int Tree::id() const { return _id; }
inline void Tree::set_id(int id) { _id = id; }
inline unsigned Tree::height() const { return _height; }
inline unsigned Tree::depth() const { return _depth; }
inline void Tree::set_size(unsigned size) { _size = size; }
inline void Tree::set_height(unsigned height) { _height = height; }
inline void Tree::set_depth(unsigned depth) { _depth = depth; }
inline Tree::PostOrderStruct Tree::postOrder() { return PostOrderStruct(this); }
inline Tree::PreOrderStruct Tree::preOrder() { return PreOrderStruct(this); }
inline unsigned Tree::size() const { return _size; }

#endif // TREE_H

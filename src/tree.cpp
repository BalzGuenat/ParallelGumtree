#include "tree.h"

Tree::Tree(int type, string label, int lineNumber)
  : _type(type), _lineNumber(lineNumber), _label(label) {}

string Tree::toString() const {
  return to_string(_type) + ':' + _label;
}

Tree::~Tree() {
  for (auto c : _children) {
      delete c;
    }
}

void Tree::getTrees(const Tree* tree, vector<const Tree*>& trees) const {
	trees.push_back(tree);
	if (!tree->isLeaf())
		for (auto child : tree->children())
			getTrees(child, trees);
}

vector<const Tree*> Tree::getTrees() const {
	vector<const Tree*> trees;
	getTrees(this, trees);
	return trees;
}

string Tree::subTreeToString() const {
  auto strings = subTreeToStringVector();
  string subTreeString;
  for (auto nodeString : strings) {
      subTreeString += nodeString + '\n';
    }
  return subTreeString;
}

vector<string> Tree::subTreeToStringVector() const {
  vector<string> strings = {toString()};
  for (auto c : _children) {
      auto subtreeStrings = c->subTreeToStringVector();
      strings.insert(strings.end(),
                     subtreeStrings.begin(),
                     subtreeStrings.end());
    }
  for (auto s = ++strings.begin(); s != strings.end(); s++)
    *s = ' ' + *s;
  return strings;
}

bool Tree::isClone(const Tree *other) const {
  if (this->_type != other->_type ||
      this->_label.compare(other->_label) != 0 ||
      this->_children.size() != other->_children.size())
    return false;
  else {
      for (unsigned i = 0; i < _children.size(); ++i) {
          if (!_children[i]->isClone(other->_children[i]))
            return false;
        }
      return true;
    }
}

Tree* Tree::clone() {
    // maybe implement with loop unrolling instead of recursively
    // queue of pairs of (newParent, oldChild)?
    Tree* newRoot = new Tree(_type, _label, _lineNumber);
    for (unsigned i=0; i != _children.size(); i++) {
        newRoot->_children.push_back(_children[i]->clone());
    }
    return newRoot;
}

int Tree::childPosition(const Tree* child) const {
  for (unsigned i = 0; i < _children.size(); ++i) {
      if (_children[i] == child)
        return i;
    }
  return -1;
}

unsigned Tree::size() const {
  // TODO optimize this by precomputation.
  unsigned size = 1;
  for (auto c : _children) {
      size += c->size();
    }
  return size;
}

Tree::PostOrderStruct::PostOrderStruct(Tree *root)
	: _root(root) {}

Tree::PostOrderStruct::PostOrderIterator Tree::PostOrderStruct::begin() {
	return PostOrderIterator(_root);
}

Tree::PostOrderStruct::PostOrderIterator Tree::PostOrderStruct::END =
		Tree::PostOrderStruct::PostOrderIterator(nullptr);

Tree::PostOrderStruct::PostOrderIterator Tree::PostOrderStruct::end() {
	return PostOrderStruct::END;
}

bool Tree::PostOrderStruct::PostOrderIterator::operator==(const PostOrderIterator& other) {
	Tree* myPtr = **this;
	Tree* otherPtr = *other;
	return myPtr == otherPtr;
}

bool Tree::PostOrderStruct::PostOrderIterator::operator!=(const PostOrderIterator& other) {
	return !(*this == other);
}

Tree* Tree::PostOrderStruct::PostOrderIterator::operator*() const {
	if (_stack.empty())
		return _root;
	auto topIt = _stack.top().second;
	return *topIt;
}

Tree::PostOrderStruct::PostOrderIterator::PostOrderIterator(Tree *tree)
	: _root(tree) {
	if (tree)
		push(tree);
}

void Tree::PostOrderStruct::PostOrderIterator::push(Tree *tree) {
	auto t = tree;
	while (!t->isLeaf()) {
		auto it = t->children().begin();
		_stack.push(make_pair(t, it));
		t = *it;
	}
}

Tree::PostOrderStruct::PostOrderIterator&
Tree::PostOrderStruct::PostOrderIterator::operator ++() {
	if (_stack.empty()) {
		if (_root)
			_root = nullptr;
		else
			throw -1;
	} else {
		auto it = ++_stack.top().second;
		if (it == _stack.top().first->children().end())
			_stack.pop();
		else
			push(*it);
	}

	return *this;
}

vector<const Tree*> Tree::descendants() const {
	vector<const Tree*> result(size() - 1);
	auto trees = getTrees();
	auto it = trees.begin();
	++it;
	for (unsigned i = 0; it != trees.end();)
		result[i++] = *it++;
	return result;
}

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

void Tree::get_trees(Tree* tree, vector<Tree*>& trees) {
	trees.push_back(tree);
	if (!tree->isLeaf())
		for (auto child : tree->children())
			get_trees(child, trees);
}

vector<Tree*> Tree::get_trees() {
	vector<Tree*> trees;
	get_trees(this, trees);
	return trees;
}

vector<Tree*> Tree::descendants() {
	vector<Tree*> descs;
	for (auto c : _children)
		get_trees(c, descs);
	return descs;
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

bool Tree::isClone(Tree *other) const {
	bool alltrue = true;
	auto thisref = this;
	#pragma omp parallel shared(alltrue, thisref)
	{
		#pragma omp single
		{
			if (thisref->_type != other->_type ||
				 thisref->_label.compare(other->_label) != 0 ||
				 thisref->_children.size() != other->_children.size())
				alltrue = false;
			else {
				for (unsigned i = 0; i < thisref->_children.size(); ++i) {
					#pragma omp task firstprivate(i) shared(thisref, alltrue)
					{
						if (!(thisref->_children[i]->isClone(other->_children[i])))
							alltrue = false;
					}
				}
				#pragma omp taskwait
			}
		}
	}
	return alltrue;
}
/*
bool Tree::isClone(Tree *other) const {
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
}*/

void Tree::refresh() {
	compute_size();
	compute_depth();
	compute_height();
}

void Tree::compute_size() {
	for (auto tree : postOrder()) {
		unsigned size = 1;
		if (!tree->isLeaf()) {
			for (auto c : tree->children())
				size += c->size();
		}
		tree->set_size(size);
	}
}

void Tree::compute_depth() {
	if (isRoot())
		_depth = 0;
	else
		_depth = parent()->depth() + 1;

	if (!isLeaf())
		for (auto c : _children)
			c->compute_depth();
}

void Tree::compute_height() {
	for (auto t : postOrder()) {
		unsigned height = 0;
		if (!t->isLeaf()) {
			for (auto c : t->children()) {
				unsigned cHeight = c->height();
				if (cHeight > height)
					height = cHeight;
			}
			height++;
		}
		t->set_height(height);
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

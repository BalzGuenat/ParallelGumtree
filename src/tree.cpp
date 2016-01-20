#include "tree.h"
#include <functional>

Tree::Tree(int type, string label, int lineNumber)
  : _type(type), _lineNumber(lineNumber), _label(label), _parent(nullptr) {}

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

const unsigned SUBTREE_SIZE_CUTOFF = 1 << 4;

bool Tree::is_clone_parallel(const Tree* a, const Tree* b) {
	if (a->_hash != b->_hash)
		return false;

	if (a->_type != b->_type ||
		 a->_label.compare(b->_label) != 0 ||
		 a->_children.size() != b->_children.size())
		return false;

	bool alltrue = true;
	for (unsigned i = 0; i < a->_children.size(); ++i)
	{
		auto ac = a->_children[i];
		auto bc = b->_children[i];
#pragma omp task shared(alltrue) if(a->size() > SUBTREE_SIZE_CUTOFF)
		{
			if (!ac->isClone(bc))
			{
#pragma omp atomic write
				alltrue = false;
#pragma omp cancel taskgroup
			}
		}
	}
#pragma omp taskwait
	return alltrue;
}

bool Tree::isClone(Tree *other) const {
	if (_hash != other->hash())
		return false;

	bool alltrue = true;
	auto this_ptr = this;
#pragma omp parallel default(shared) if(size() > SUBTREE_SIZE_CUTOFF)
	{
#pragma omp master
		{
#pragma omp taskgroup
			{
				alltrue = is_clone_parallel(this_ptr, other);
			}
		}
	}
	return alltrue;
}

void Tree::refresh() {
	compute_size();
	compute_depth();
	compute_height();
	compute_hash();
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

void Tree::compute_hash() {
	for (auto c : _children) {
#pragma omp task
		c->compute_hash();
	}
#pragma omp taskwait
	std::hash<string> str_hash;
	std::hash<unsigned> u_hash;
	unsigned h = u_hash(u_hash(_type) ^ str_hash(_label));
	for (auto c : _children) {
		h = u_hash(h ^ c->hash());
	}
	_hash = h;
}

Tree* Tree::clone() {
	auto clone = new Tree(_type, _label, _lineNumber);
	if (!isLeaf()) {
		for (auto child : _children) {
			auto childClone = child->clone();
			childClone->_parent = clone;
			clone->_children.push_back(childClone);
		}
	}
	return clone;
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

Tree::PreOrderStruct::PreOrderStruct(Tree *root)
	: _root(root) {}

Tree::PreOrderStruct::PreOrderIterator Tree::PreOrderStruct::begin() {
	return PreOrderIterator(_root);
}

Tree::PreOrderStruct::PreOrderIterator Tree::PreOrderStruct::END =
		Tree::PreOrderStruct::PreOrderIterator(nullptr);

Tree::PreOrderStruct::PreOrderIterator Tree::PreOrderStruct::end() {
	return PreOrderStruct::END;
}

bool Tree::PreOrderStruct::PreOrderIterator::operator==(const PreOrderIterator& other) {
	Tree* myPtr = **this;
	Tree* otherPtr = *other;
	return myPtr == otherPtr;
}

bool Tree::PreOrderStruct::PreOrderIterator::operator!=(const PreOrderIterator& other) {
	return !(*this == other);
}

Tree* Tree::PreOrderStruct::PreOrderIterator::operator*() const {
	if (_stack.empty())
		return nullptr;
	return _stack.top().first;
}

Tree::PreOrderStruct::PreOrderIterator::PreOrderIterator(Tree *tree) {
	if (tree) {
		auto new_top = make_pair(tree, tree->children().cbegin());
		_stack.push(new_top);
	}
}

Tree::PreOrderStruct::PreOrderIterator&
Tree::PreOrderStruct::PreOrderIterator::operator ++() {
	while (_stack.size() > 0 && _stack.top().second == _stack.top().first->children().cend())
		_stack.pop();
	if (_stack.size() > 0) {
		Tree* new_top = *(_stack.top().second++);
		_stack.push(make_pair(new_top, new_top->children().cbegin()));
	}
	return *this;
}

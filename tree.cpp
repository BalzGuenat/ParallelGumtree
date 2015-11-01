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

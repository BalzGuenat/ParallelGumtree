#include "fileparser.h"
#include <vector>
#include <stack>
#include <fstream>
#include <cwctype>

//FileParser::FileParser() {}


Tree* FileParser::parse(string filepath) {
  ifstream file(filepath);
  unsigned lineNumber = 0;
  stack<Tree*> parents;
  for (string line; getline(file, line); ) {
      unsigned depth = 0; // TODO compute correctly. This is the indentation level.
      while (iswspace(line[depth++]));
      depth--;

      // We walk up the tree until we are at the right depth.
      while (parents.size() > depth)
        parents.pop();

      auto parent = parents.empty() ? nullptr : parents.top();
		auto tree = parseLine(parent, line, ++lineNumber);
		tree->set_depth(depth);
      parents.push(tree);
    }
  while (parents.size() > 1)
    parents.pop();
  file.close();

  // post order numbering and height
  int id = 0;
  for (auto t : parents.top()->postOrder()) {
	  t->set_id(id++);
	  unsigned height = 0;
	  if (!t->isLeaf()) {
		  for (auto c : t->children()) {
			  if (c->height() > height)
				  height = c->height();
		  }
		  ++height;
	  }
	  t->set_height(height);
  }

  if (parents.empty())
	  return nullptr;
  else {
	  auto root = parents.top();
	  root->refresh();
	  return root;
  }
}

vector<string> split(string s, char c) {
  vector<string> result;
  int last = 0;
  int cur = 0;
  for (auto sc : s) {
      if (sc == c) {
          result.push_back(s.substr(last, cur));
          last = cur + 1;
        }
      cur++;
    }
  result.push_back(s.substr(last, cur));
  return result;
}

Tree* FileParser::parseLine(Tree* parent, string line, int lineNumber) {
  auto fields = split(line, ':');
  Tree* tree = new Tree(stoi(fields[0]), fields[1], lineNumber);
  tree->set_id(lineNumber);
  tree->_parent = parent;
  if (parent)
    parent->_children.push_back(tree);
  return tree;
}


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

      // We walk up the tree until we are at the right depth.
      while (parents.size() > depth)
        parents.pop();

      auto parent = parents.empty() ? nullptr : parents.top();
      auto tree = parseLine(parent, line, ++lineNumber);
      // TODO check that top() on empty stack return nullptr.
      parents.push(tree);
    }
  while (parents.size() > 1)
    parents.pop();
  return parents.empty() ? nullptr : parents.top();
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
  tree->_parent = parent;
  if (parent)
    parent->_children.push_back(tree);
  return tree;
}


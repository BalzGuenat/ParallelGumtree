#ifndef TREECREATOR_H
#define TREECREATOR_H

#include <string>
#include "tree.h"
using namespace std;

class TreeCreator
{
public:
  static void pruferTrees(unsigned nodeNumber, string filepath);
private:
  static Tree* seq2Tree(unsigned nodeNumber);
  static Tree* modifyTreeRandom(unsigned initNodeNumber, unsigned targetNodeNumber, Tree* root);
  static void addNodes(Tree* root, vector<Tree*> &nodes);
  static unsigned label_counter;
  static int rNumber();
  static string nextLabel();


  // "Settings"
  static const unsigned maxTypes = 5;

};

inline int TreeCreator::rNumber() { return rand() % TreeCreator::maxTypes;}
inline string TreeCreator::nextLabel() {
  return "Label " + (std::to_string(label_counter++));
}

#endif // TREECREATOR_H

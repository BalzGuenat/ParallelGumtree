#ifndef TREECREATOR_H
#define TREECREATOR_H

#include <string>
#include <tree.h>
using namespace std;

class TreeCreator
{
public:
  static void newTrees(unsigned depth, unsigned maxChildren, string filepath);
  static void pruferTrees(unsigned nodeNumber, string filepath);
private:
  static Tree* seq2Tree(vector<unsigned> sequence, unsigned nodeNumber, Tree* nodes[]);
  static Tree* modifiedTree(vector<unsigned> sequence, unsigned oldNodeNumber, Tree* oldNodes[]);
  static unsigned label_counter;
  static string labels[100];
  static int rNumber();
  static string nextLabel();


  // "Settings"
  static const bool minHalfFull = false;
  static const unsigned maxTypes = 100;
  static const unsigned maxLabelLength = 50;
  static const double varySize; // by how much may the tree size change
  static const double permNumb; // how many permuations? (ex. 1/4th the number of nodes)

};

inline int TreeCreator::rNumber() { return rand() % TreeCreator::maxTypes;}
inline string TreeCreator::nextLabel() {
  return "Label " + (std::to_string(label_counter++));
}

#endif // TREECREATOR_H

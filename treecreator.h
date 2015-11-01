#ifndef TREECREATOR_H
#define TREECREATOR_H

#include <string>
using namespace std;

class TreeCreator
{
public:
  static void newTrees(unsigned depth, unsigned maxChildren, string filepath);
private:
  static void createLabels();
  static unsigned label_counter;
  static string labels[100];
  static int rNumber();
  static string rLabel();


  // "Settings"
  static const bool minHalfFull = true;
  static const unsigned maxTypes = 100;
  static const unsigned maxLabelLength = 50;
  static const string alphanum;

};

unsigned TreeCreator::label_counter = 99;
const string TreeCreator::alphanum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

inline int TreeCreator::rNumber() { return rand() % TreeCreator::maxTypes;}
inline string TreeCreator::rLabel() {
  if (TreeCreator::label_counter==99) createLabels();
  TreeCreator::label_counter++;
  return TreeCreator::labels[TreeCreator::label_counter];
}

#endif // TREECREATOR_H

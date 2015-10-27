#include "treecreator.h"
#include <tree.h>
#include <filewriter.h>
#include <vector>
#include <time.h> // time
#include <stdlib.h> // srand, rand
#include <stdio.h>

using namespace std;

static unsigned label_counter = 99;
// "Settings"
static const bool minHalfFull = true;
static const unsigned maxTypes = 100;
static const unsigned maxLabelLength = 50;
static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

void TreeCreater::createLabels(){
  unsigned alphanumSize = sizeof(TreeCreator::alphanum) - 1;
  TreeCreator::label_counter = 0; // reset label counter
  for (int j = 0; j<100; j++) { // create 100 new labels
      char s[TreeCreator::maxLabelLength];
      for (int i = 0; i < (rand() % TreeCreator::maxLabelLength-1)+1; ++i) { // label length, minimally 1
          s[i] = TreeCreator::alphanum[rand() % alphanumSize];
        }
      s[TreeCreator::maxLabelLength] = 0;
      TreeCreator::labels[j] = s;
    }
}

void TreeCreater::newTrees(unsigned depth, unsigned maxChildren, string filepath){
  srand (time(NULL)); // get new random seed
  maxNumberTypes = maxTypes;

  vector<Tree *> nodes[depth]; // stores pointers to each node on each level
  nodes[0].push_back(new Tree(rNumber(), rLabel(), 0)); //
  //for (int i = 0; i < 100; i++) printf(rLabel()+"\n");

  // iterate over all depth and create children on each
  for (unsigned curDepth = 0; curDepth < depth; curDepth++){

      // iterate over all trees on the current depth
      for (vector<Tree *>::iterator it = nodes[curDepth].begin(); it != nodes[curDepth].end(); ++it) {

          // create random number of children for this one each
          unsigned numbChild;
          if (TreeCreator::minHalfFull){
              numbChild = 1 + ((maxChildren -1) / 2); // ceil(maxChildren/2)
              numbChild =  numbChild + (rand() % numbChild);
            }
          else {
              numbChild = rand() % maxChildren;
            }
          for (unsigned n = 0; n < numbChild; n++) {
              Tree t = new Tree(rNumber(), rLabel(), 0);
              it.children.push_back(t);
              nodes[curDepth+1].push_back(t);
            }
        }
    }

  FileWriter::write(nodes[0][0],filepath+"0");
}

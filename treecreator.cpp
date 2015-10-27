#include "treecreator.h"
#include <tree.h>
#include <filewriter.h>
#include <vector>
#include <time.h> // time
#include <stdlib.h> // srand, rand
#include <stdio.h>


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
      for (int i = 0; i < (rand() % TreeCreator::maxLabelLength+1); ++i) {
          s[i] = TreeCreator::alphanum[rand() % alphanumSize];
        }
      s[TreeCreator::maxLabelLength] = 0;
      TreeCreator::labels[j] = s;
    }
}

void TreeCreater::newTrees(unsigned depth, unsigned maxChildren, string filepath){
  srand (time(NULL)); // get new random seed
  maxNumberTypes = maxTypes;
  unsigned line = 0;
  unsigned curDepth = 0;

  vector<Tree *> nodes;
  Tree firstT = new Tree(rNumber(), rLabel(), 0);
  nodes.push_back(firstT);
  //for (int i = 0; i < 100; i++) printf(rLabel()+"\n");


  Tree secondT = new Tree(rNumber(), rLabel(), 0);
}

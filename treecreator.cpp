#include "treecreator.h"
#include <tree.h>
#include <filewriter.h>
#include <vector>
#include <time.h> // time
#include <stdlib.h> // srand, rand
#include <stdio.h>

using namespace std;

void TreeCreator::createLabels(){
  /* TODO Chris, this method seems extremely error prone, please absolutely test this...
   * s is a local variable and stored on the stack but you store its position into a static data structure.
   * in the inner loop, you evaluate rand() each loop. Is this on purpose?
   * You don't null-terminate a string after what seems to be the randomly chosen label length but
   * rather always after maxLabelLength.
   * Try to work with the string class instead of char[]. It makes lots of stuff easier and safer.
   * I do realize you might not be able to do that as you randomly assign each character in the string.
   * Why do you generate labels randomly in the first place? Why not just have a counter that counts up?
   * That way you even get the guarantee that you don't repeat labels and it's much easier.
   */
  unsigned alphanumSize = sizeof(TreeCreator::alphanum) - 1;
  TreeCreator::label_counter = 0; // reset label counter
  for (int j = 0; j<100; j++) { // create 100 new labels
      char s[TreeCreator::maxLabelLength];
      for (unsigned i = 0; i < (rand() % TreeCreator::maxLabelLength-1)+1; ++i) { // label length, minimally 1
          s[i] = TreeCreator::alphanum[rand() % alphanumSize];
        }
      s[TreeCreator::maxLabelLength] = 0;
      TreeCreator::labels[j] = s;
    }
}

void TreeCreator::newTrees(unsigned depth, unsigned maxChildren, string filepath){
  srand (time(NULL)); // get new random seed

  vector<Tree *> nodes[depth]; // stores pointers to each node on each level
  nodes[0].push_back(new Tree(rNumber(), rLabel(), 0)); //
  //for (int i = 0; i < 100; i++) printf(rLabel()+"\n");

  // iterate over all depth and create children on each
  for (unsigned curDepth = 0; curDepth < depth; curDepth++){

      // iterate over all trees on the current depth
      for (auto currentDepthNode : nodes[curDepth]) {

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
              auto t = new Tree(rNumber(), rLabel(), 0);
              t->_parent = currentDepthNode;
              currentDepthNode->_children.push_back(t);
              nodes[curDepth+1].push_back(t);
            }
        }
    }

  FileWriter::write(nodes[0][0],filepath+"0");
}

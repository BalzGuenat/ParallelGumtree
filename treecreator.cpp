#include "treecreator.h"
#include <tree.h>
#include <filewriter.h>
#include <vector>
#include <time.h> // time
#include <stdlib.h> // srand, rand
#include <stdio.h>
#include <algorithm>
#include <iostream>

using namespace std;

unsigned TreeCreator::label_counter = 0;
const double TreeCreator::varySize = 0.25; // by how much may the tree size change
const double TreeCreator::permNumb = 0.15; // how many permuations? (ex. 1/4th the number of nodes)

void TreeCreator::newTrees(unsigned depth, unsigned maxChildren, string filepath){
  srand (time(NULL)); // get new random seed

  vector<Tree *> nodes[depth]; // stores pointers to each node on each level
  nodes[0].push_back(new Tree(rNumber(), nextLabel(), 0)); //
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
              auto t = new Tree(rNumber(), nextLabel(), 0);
              t->_parent = currentDepthNode;
              currentDepthNode->_children.push_back(t);
              nodes[curDepth+1].push_back(t);
            }
        }
    }

  FileWriter::write(nodes[0][0],filepath+"0");
}

Tree* TreeCreator::seq2Tree(vector<unsigned> sequence, unsigned nodeNumber, Tree *nodes[]){

  // create Prufer Sequence
  unsigned degree [nodeNumber];
  // sequence are random numbers between 0 and n-1
  // degree[i] = number of occurences of i in the sequence + 1
  for (unsigned i = 0; i < nodeNumber; i++) {
      degree[i] = 1;
    }
  for (unsigned i = 0; i <nodeNumber-2; i++) {
      degree[sequence[i]]++;
    }
  for (unsigned i = 0; i < nodeNumber; i++) {
    nodes[i] = new Tree(rNumber(), "Label "+ (std::to_string(i)), 0);
    }
  unsigned ptr = 0;
  while (degree[ptr] != (unsigned)1)
    ptr++;
  unsigned leaf = ptr;
  for (unsigned i = 0; i < nodeNumber-2; i++) {
      unsigned v = sequence[i];
      nodes[leaf]->_parent = nodes[v];
      nodes[v]->_children.push_back(nodes[leaf]);
      degree[leaf]--;
      degree[v]--;
      if (degree[v]==(unsigned)1 && v < ptr) {
          leaf = v;
        }
      else {
          for (++ptr; ptr < nodeNumber && degree[ptr] != 1; ++ptr);
          leaf = ptr;
        }
    }
  for (unsigned v = 0; v < nodeNumber - 1; v++) {
      if (degree[v] == 1) {
          nodes[v]->_children.push_back(nodes[nodeNumber-1]);
          nodes[nodeNumber-1]->_parent = nodes[v];
          ptr = v;
        }
    }
  return nodes[ptr];
}

Tree* TreeCreator::modifiedTree(vector<unsigned> sequence, unsigned oldNodeNumber, Tree* oldNodes[]) {
  unsigned nodeNumber = unsigned((int)oldNodeNumber + (rand() % (int)((oldNodeNumber*varySize)-oldNodeNumber*(varySize/2))));

  sequence.resize(nodeNumber);
  if (oldNodeNumber < nodeNumber) {
      for (unsigned i = oldNodeNumber; i < nodeNumber-2; i++)
        sequence[i] = (unsigned) (rand() % (nodeNumber-1));
    }
  for (unsigned i = 0; i < permNumb*nodeNumber; i++) {
      unsigned r1 = rand() % nodeNumber;
      unsigned r2 = rand() % nodeNumber;
      iter_swap(sequence.begin() + r1, sequence.begin()+ r2);
    }

  // ---------
  unsigned degree [nodeNumber];
  // sequence are random numbers between 0 and n-1
  // degree[i] = number of occurences of i in the sequence + 1
  for (unsigned i = 0; i < nodeNumber; i++) {
      degree[i] = 1;
    }
  for (unsigned i = 0; i <nodeNumber-2; i++) {
      degree[sequence[i]]++;
    }
  Tree* nodes[nodeNumber];
  for (unsigned i = 0; i < oldNodeNumber; i++) {
      nodes[i] = oldNodes[i];
      nodes[i]->_children = {};
      nodes[i]->_parent = NULL;
    }
  for (unsigned i = oldNodeNumber; i < nodeNumber; i++) {
    nodes[i] = new Tree(rNumber(), "Label "+ (std::to_string(i)), 0);
    }
  unsigned ptr = 0;
  while (degree[ptr] != (unsigned)1)
    ptr++;
  unsigned leaf = ptr;
  cout << "t1" << endl;
  for (unsigned i = 0; i < nodeNumber-2; i++) {
      unsigned v = sequence[i];
      cout << "t2" << endl;
      cout << v << endl;
      nodes[leaf]->_parent = nodes[v];
      nodes[v]->_children.push_back(nodes[leaf]);
      degree[leaf]--;
      degree[v]--;
      if (degree[v]==(unsigned)1 && v < ptr) {
          leaf = v;
        }
      else {
          for (++ptr; ptr < nodeNumber && degree[ptr] != 1; ++ptr);
          leaf = ptr;
        }
    }
  cout << "t3" << endl;
  for (unsigned v = 0; v < nodeNumber - 1; v++) {
      if (degree[v] == 1) {
          nodes[v]->_children.push_back(nodes[nodeNumber-1]);
          nodes[nodeNumber-1]->_parent = nodes[v];
          ptr = v;
        }
    }
  return nodes[ptr];
}

void TreeCreator::pruferTrees(unsigned nodeNumber, string filepath) {
  srand (time(NULL));
  vector<unsigned> sequence(nodeNumber-2);
  for (unsigned i = 0; i < nodeNumber-2; i++)
    sequence[i] = (unsigned) (rand() % (nodeNumber-1));
  Tree* nodes [nodeNumber];
  Tree* root1 = seq2Tree(sequence, nodeNumber, nodes);
  FileWriter::write(root1, filepath+"1");
  Tree* root2 = modifiedTree(sequence, nodeNumber, nodes);
  FileWriter::write(root2, filepath+"2");
}

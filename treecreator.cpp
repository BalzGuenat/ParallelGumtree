#include "treecreator.h"
#include <tree.h>
#include <filewriter.h>
#include <vector>
#include <time.h> // time
#include <stdlib.h> // srand, rand
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <stack>

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

Tree* TreeCreator::seq2Tree(unsigned nodeNumber){
    vector<Tree*> nodes (nodeNumber, NULL);
  // create Prufer Sequence
    vector<unsigned> sequence(nodeNumber-2);
    for (unsigned i = 0; i < nodeNumber-2; i++)
        sequence[i] = (unsigned) (rand() % (nodeNumber-1));

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


void TreeCreator::pruferTrees(unsigned nodeNumber, string filepath) {
  srand (time(NULL));
  unsigned initNodeNumber = nodeNumber/20;
  initNodeNumber = initNodeNumber > 2 ? initNodeNumber : 3;
  Tree* root1 = seq2Tree(initNodeNumber);
  Tree* root2 = modifyTreeRandom(initNodeNumber, nodeNumber, root1);
  FileWriter::write(root1, filepath+"1");
  FileWriter::write(root2, filepath+"2");
}

const double delChance = 20; // 1/20 chance, same for the ones below
const double moveChance = 20;
const double swapChance = 20;
const double labelEditChance = 20;
const double typeEditChance = 20;
const double copyChance = 20;



Tree* TreeCreator::modifyTreeRandom(unsigned initNodeNumber, unsigned targetNodeNumber, Tree* root1) {
    Tree* root2 = root1->clone(); // clone tree 1

    // create vectors of the nodes
    vector<Tree*> nodes2;
    addNodes(root2, nodes2);
    vector<Tree*> nodes1;
    addNodes(root1, nodes1);

    int added1 = 0; // how many nodes have we added
    int added2 = 0;
    // both trees have more than targetNodeNumber/2 nodes and 1 has more than targetNodeNumber
    while ((added1+initNodeNumber > targetNodeNumber || added2+initNodeNumber > targetNodeNumber) && (added1+initNodeNumber > targetNodeNumber/2 && added2+initNodeNumber > targetNodeNumber/2)) {
        unsigned n = rand() % initNodeNumber; // which node
        unsigned changeSize = initNodeNumber/2 + (rand() % initNodeNumber);
        Tree* nT;
        unsigned n2;
        Tree* nT2;
        switch (rand() % 4) {
            case 0: {// "delete" (add to 1 only)
                nT = seq2Tree(changeSize);
                if (rand() % 2 == 0) {
                    nodes1[n]->_children.push_back(nT);
                    added1+=changeSize;
                    addNodes(nT, nodes1);
                }
                else {
                    nodes2[n]->_children.push_back(seq2Tree(changeSize));
                    added2+=changeSize;
                    addNodes(nT, nodes2);
                }
                break;
        }
            case 1: {// add to both
                nT = seq2Tree(changeSize);
                nodes1[n]->_children.push_back(nT);
                added1+=changeSize;
                addNodes(nT, nodes1);
                nodes2[n]->_children.push_back(seq2Tree(changeSize));
                added2+=changeSize;
                addNodes(nT, nodes2);
                break;
        }
            case 2: {// swap
                while ((n2 = rand() % initNodeNumber) == n);
                unsigned changeSize2 = initNodeNumber/2 + (rand() % initNodeNumber);
                nT = seq2Tree(changeSize);
                nT2 = seq2Tree(changeSize2);

                // add nT
                nodes1[n]->_children.push_back(nT);
                added1+=changeSize;
                addNodes(nT, nodes2);
                nodes2[n2]->_children.push_back(nT);
                added2+=changeSize;
                addNodes(nT, nodes2);

                //add nT2
                nodes1[n2]->_children.push_back(nT2);
                added1+=changeSize2;
                addNodes(nT2, nodes2);
                nodes2[n]->_children.push_back(nT2);
                added2+=changeSize2;
                addNodes(nT2, nodes1);
                break;
        }
            case 3: {// copy (add to 1 twice)
                nT = seq2Tree(changeSize);
                nodes1[n]->_children.push_back(nT);
                added1+=changeSize;
                addNodes(nT, nodes1);
                nodes2[n]->_children.push_back(seq2Tree(changeSize));
                added2+=changeSize;
                addNodes(nT, nodes2);
                n2 = rand() % initNodeNumber;
                if (rand() % 2 == 0) {
                    nodes1[n2]->_children.push_back(nT);
                    added1+=changeSize;
                    addNodes(nT, nodes1);
                }
                else {
                    nodes2[n2]->_children.push_back(seq2Tree(changeSize));
                    added2+=changeSize;
                    addNodes(nT, nodes2);
                }
                break;
        }
        }

        // change label names
        unsigned edit1 = rand() % (nodes1.size()/2);
        unsigned edit2 = rand() % (nodes2.size()/2);
        unsigned max = nodes1.size() > nodes2.size() ? nodes1.size() : nodes2.size();
        max = max + max/2;
        for (unsigned i = 0; i < edit1; i++) {
            unsigned n = rand() % nodes1.size();
            nodes1[n] = new Tree(nodes1[n]->type(), "Label " + (std::to_string(rand() % max)), 0);
        }
        for (unsigned i = 0; i < edit2; i++) {
            unsigned n = rand() % nodes2.size();
            nodes2[n] = new Tree(nodes2[n]->type(), "Label " + (std::to_string(rand() % max)), 0);
        }

        // change type numbers
        edit1 = rand() % (nodes1.size()/2);
        edit2 = rand() % (nodes2.size()/2);
        for (unsigned i = 0; i < edit1; i++) {
            unsigned n = rand() % nodes1.size();
            nodes1[n] = new Tree(rNumber(), nodes1[n]->label(), 0);
        }
        for (unsigned i = 0; i < edit2; i++) {
            unsigned n = rand() % nodes2.size();
            nodes2[n] = new Tree(rNumber(), nodes2[n]->label(), 0);
        }

    }
    return root2;
}

void TreeCreator::addNodes(Tree* root, vector<Tree*> nodes) {
    stack<Tree*> listForChildren;
    listForChildren.push(root);
    while (!listForChildren.empty()) {
        Tree* next = listForChildren.top();
        listForChildren.pop();
        for (unsigned i = 0; i < next->_children.size(); i++)
            listForChildren.push(next->children()[i]);
        nodes.push_back(next);
    }
}

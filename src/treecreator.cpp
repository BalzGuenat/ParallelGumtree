#include "treecreator.h"
#include "tree.h"
#include "filewriter.h"
#include <vector>
#include <time.h> // time
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <stack>

using namespace std;

unsigned TreeCreator::label_counter = 0;

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
	 nodes[i] = new Tree(rNumber(), "label"+ (std::to_string(i)), 0);
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

Tree* TreeCreator::myTreeGen(unsigned nodeNumber) {
	label_counter = 0;
	const double DOWN_CHANCE = 0.25;
	const double SIBLING_CHANCE = DOWN_CHANCE + 0.5;
	const double UP_CHANCE = SIBLING_CHANCE + 0.25;
	stack<Tree*> parents({new Tree(rNumber(), nextLabel(), 0)});
	while (label_counter < nodeNumber) {
		double dice = ((double) (rand() % 100)) / 100;
		auto newTree = new Tree(rNumber(), nextLabel(), 0);

		if (dice < DOWN_CHANCE || parents.size() == 1) {
			newTree->_parent = parents.top();
			parents.top()->_children.push_back(newTree);
			parents.push(newTree);
		} else if (dice < SIBLING_CHANCE || parents.size() == 2) {
			parents.pop();
			newTree->_parent = parents.top();
			parents.top()->_children.push_back(newTree);
			parents.push(newTree);
		} else if (dice < UP_CHANCE) {
			parents.pop();
			parents.pop();
		}
	}
	while (parents.size() > 1)
		parents.pop();
	return parents.top();
}


void TreeCreator::pruferTrees(unsigned nodeNumber, string filepath) {
  srand (time(NULL));
  unsigned initNodeNumber = nodeNumber/5;
  initNodeNumber = initNodeNumber > 2 ? initNodeNumber : 3;
//  Tree* root1 = seq2Tree(initNodeNumber);
  Tree* root1 = myTreeGen(initNodeNumber);
  cerr << "First tree generated" << endl;
  Tree* root2 = modifyTreeRandom(initNodeNumber, nodeNumber, root1);
  cerr << "Second tree generated" << endl;
  FileWriter::write(root1, filepath+"1");
  FileWriter::write(root2, filepath+"2");
}



Tree* TreeCreator::modifyTreeRandom(unsigned initNodeNumber, unsigned targetNodeNumber, Tree* root1) {
    Tree* root2 = root1->clone(); // clone tree 1

	 cerr << "starting modifications" << endl;

    // create vectors of the nodes
    vector<Tree*> nodes2;
    addNodes(root2, nodes2);
    vector<Tree*> nodes1;
    addNodes(root1, nodes1);

    unsigned added1 = 0; // how many nodes have we added
    unsigned added2 = 0;
    // both trees have more than targetNodeNumber/2 nodes and 1 has more than targetNodeNumber
	 while ((added1+initNodeNumber < targetNodeNumber && added2+initNodeNumber < targetNodeNumber) ||
			  (added1+initNodeNumber < targetNodeNumber/2 || added2+initNodeNumber < targetNodeNumber/2)) {
		  unsigned n = rand() % min(nodes1.size(), nodes2.size()); // which node
        unsigned changeSize = initNodeNumber/2 + (rand() % initNodeNumber);
        Tree* nT;
        unsigned n2;
        Tree* nT2;
        switch (rand() % 4) {
            case 0: {// "delete" (add to 1 only)

				  cerr << "case 0" << endl;
					 nT = myTreeGen(changeSize);
                if (rand() % 2 == 0) {
                    nodes1[n]->_children.push_back(nT);
                    nT->_parent = nodes1[n];
                    added1+=changeSize;
                    addNodes(nT, nodes1);
                }
                else {
                    nodes2[n]->_children.push_back(nT);
                    nT->_parent = nodes2[n];
                    added2+=changeSize;
                    addNodes(nT, nodes2);
                }
                break;
        }
            case 1: {// add to both

				  cerr << "case 1" << endl;
					 nT = myTreeGen(changeSize);
                nodes1[n]->_children.push_back(nT);
                added1+=changeSize;
                addNodes(nT, nodes1);
                nT->_parent = nodes1[n];

                nT = nT->clone();
					 nodes2[n]->_children.push_back(myTreeGen(changeSize));
                added2+=changeSize;
                addNodes(nT, nodes2);
                nT->_parent = nodes2[n];
                break;
        }
            case 2: {// swap

				  cerr << "case 2" << endl;
                while ((n2 = rand() % initNodeNumber) == n);
                unsigned changeSize2 = initNodeNumber/2 + (rand() % initNodeNumber);
					 nT = myTreeGen(changeSize);
					 nT2 = myTreeGen(changeSize2);

                // add nT
                nodes1[n]->_children.push_back(nT);
                added1+=changeSize;
                addNodes(nT, nodes1);
                nT->_parent = nodes1[n];

                nT = nT->clone();
                nodes2[n2]->_children.push_back(nT);
                added2+=changeSize;
                addNodes(nT, nodes2);
                nT->_parent = nodes2[n2];

                //add nT2
                nodes1[n2]->_children.push_back(nT2);
                added1+=changeSize2;
                addNodes(nT2, nodes1);
                nT2->_parent = nodes1[n2];

                nT2 = nT2->clone();
                nodes2[n]->_children.push_back(nT2);
                added2+=changeSize2;
                addNodes(nT2, nodes2);
                nT2->_parent = nodes2[n2];
                break;
        }
            case 3: {// copy (add to 1 twice)

				  cerr << "case 3" << endl;
					 nT = myTreeGen(changeSize);
                nodes1[n]->_children.push_back(nT);
                added1+=changeSize;
                addNodes(nT, nodes1);
                nT->_parent = nodes1[n];

                nT = nT->clone();
                nodes2[n]->_children.push_back(nT);
                added2+=changeSize;
                addNodes(nT, nodes2);
                nT->_parent = nodes2[n];

                n2 = rand() % initNodeNumber;
                nT = nT->clone();
                if (rand() % 2 == 0) {
                    nodes1[n2]->_children.push_back(nT);
                    added1+=changeSize;
                    addNodes(nT, nodes1);
                    nT->_parent = nodes1[n2];
                }
                else {
						  nodes2[n2]->_children.push_back(myTreeGen(changeSize));
                    added2+=changeSize;
                    addNodes(nT, nodes2);
                    nT->_parent = nodes2[n2];
                }
                break;
        }
        }
    }

	 cerr << "after mod loop" << endl;

    // change label names
    unsigned edit1;
    unsigned edit2;
    if (nodes1.size() < 100)
        edit1 = 0;
    else
        edit1 = rand() % (nodes1.size()/100);
    if (nodes2.size() < 100)
        edit2 = 0;
    else
        edit2 = rand() % (nodes2.size()/100);
    cout << "tree sizes: " << nodes1.size() << " and " << nodes2.size() << endl;
    cout << "label changes: " << edit1 << " and " << edit2 << endl;
    unsigned max = nodes1.size() > nodes2.size() ? nodes1.size() : nodes2.size();
    max = max + max/2;
    for (unsigned i = 0; i < edit1; i++) {
        unsigned n = rand() % nodes1.size();
        nodes1[n] = new Tree(nodes1[n]->type(), "label" + (std::to_string(rand() % max)), 0);
    }
    for (unsigned i = 0; i < edit2; i++) {
        unsigned n = rand() % nodes2.size();
        nodes2[n] = new Tree(nodes2[n]->type(), "label" + (std::to_string(rand() % max)), 0);
    }

    // change type numbers
    if (nodes1.size() < 100)
        edit1 = 0;
    else
        edit1 = rand() % (nodes1.size()/100);
    if (nodes2.size() < 100)
        edit2 = 0;
    else
        edit2 = rand() % (nodes2.size()/100);
    cout << "type changes: " << edit1 << " and " << edit2 << endl;
    for (unsigned i = 0; i < edit1; i++) {
        unsigned n = rand() % nodes1.size();
        nodes1[n] = new Tree(rNumber(), nodes1[n]->label(), 0);
    }
    for (unsigned i = 0; i < edit2; i++) {
        unsigned n = rand() % nodes2.size();
        nodes2[n] = new Tree(rNumber(), nodes2[n]->label(), 0);
    }

    // tall or bushy?
    unsigned sum = 0;
	 unsigned non_leaves = 0;
	 for (auto t : nodes1) {
		  if (!t->isLeaf()) {
				sum += t->children().size();
				++non_leaves;
		  }
    }
	 cout << "average children number of file1: " << (double)sum/(double)non_leaves << endl;
	 sum = 0;
	 non_leaves = 0;
	 for (auto t : nodes2) {
		  if (!t->isLeaf()) {
				sum += t->children().size();
				++non_leaves;
		  }
	 }
	 cout << "average children number of file2: " << (double)sum/(double)non_leaves << endl;


    return root2;
}

void TreeCreator::addNodes(Tree* root, vector<Tree*> &nodes) {
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

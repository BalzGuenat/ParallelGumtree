#include <iostream>
#include "tree.h"
#include "fileparser.h"
#include "classicgumtree.h"
#include "filewriter.h"
#include "treecreator.h"
#include "mappingstore.h"

using namespace std;

const int DEFAULT_PRUFERTREE_SIZE = 1000;
const string USAGE_TEXT = "ParallelGumtree\n"
								  "Usage: ParallelGumtree FILE0 [FILE1]\n"
								  "       ParallelGumtree -create [NUMBER]\n"
								  "\n"
								  "where FILEn encodes a tree Tn.\n"
								  "If FILE1 is provided then a matching between T0 and T1 "
								  "is computed and printed to stdout.\n"
								  "If FILE1 is not provided then FILE0 is parsed, T0 is built "
								  "internally and then written to stdout and \"FILE1.replica\".\n"
								  "If ParallelGumtree is called with the -create option, "
								  "pruferTrees is called which creates two similar trees "
								  "in the working directory \ncalled randomPrufer1 and randomPrufer2. "
								  "If NUMBER is provided (and NUMBER>=10) for -create, the created trees will have about that\n"
								  "many nodes. Otherwise it will have the default size of about " + to_string(DEFAULT_PRUFERTREE_SIZE) + " nodes.";

int main(int argc, char* argv[])
{
	// make args into proper strings
	vector<string> args(argc);
	for (int i = 0; i < argc; ++i)
		args[i] = argv[i];

	if (argc < 2) {
		cout << USAGE_TEXT << endl;
		return 0;

	} else if (argc == 2 && args[1] == "-create") {
		TreeCreator::pruferTrees(DEFAULT_PRUFERTREE_SIZE, "randomPrufer");
		return 0;

	} else if (argc == 2) {
		// one file was passed, replicate it
		auto file0 = args[1];
		auto t = FileParser::parse(file0);
		cout << t->subTreeToString() << endl;
		FileWriter::write(t, file0 + ".replica");
		cout << "PostOrder:" << endl;
		for (auto tt : t->postOrder())
			cout << tt->toString() << endl;
		delete t; t = nullptr;
		return 0;

	} else if (argc == 3 && args[1] == "-create") {
		int size;
		try {
			size = stoi(args[2], NULL, 10); // convert
		} catch (const invalid_argument&) {
			size = 0;
		} if (size<10) {
			cout << "The argument NUMBER is smaller than 10 or not an integer. The default size of " << DEFAULT_PRUFERTREE_SIZE << " was taken to create the tree." << endl;
			TreeCreator::pruferTrees(DEFAULT_PRUFERTREE_SIZE, "randomPrufer");
		  } else {
			TreeCreator::pruferTrees(size, "randomPrufer");
		  }
		return 0;
	} else if (argc == 3) {
		// two files were passed, match them
		auto file0 = args[1];
		auto file1 = args[2];
		auto t0 = FileParser::parse(file0);
		auto t1 = FileParser::parse(file1);
		// TODO match trees.
		MappingStore mapping;
		ClassicGumtree matcher(t0, t1, &mapping);
		matcher.match();
		for (auto m : mapping.asSet())
			cout << m.first->lineNumber() << " -> " << m.second->lineNumber() << endl;
		delete t0; t0 = nullptr;
		delete t1; t1 = nullptr;
		return 0;
	}
	return -1;
}


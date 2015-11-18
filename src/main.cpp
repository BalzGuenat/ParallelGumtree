#include <iostream>
#include "tree.h"
#include "fileparser.h"
#include "matcher.h"
#include "filewriter.h"
#include "treecreator.h"
#include "mappingstore.h"

using namespace std;

// TODO @Chris: Please add a better description of what happens with -create.
const string USAGE_TEXT = "ParallelGumtree\n"
								  "Usage: ParallelGumtree FILE0 [FILE1]\n"
								  "       ParallelGumtree -create\n"
								  "\n"
								  "where FILEn encodes a tree Tn.\n"
								  "If FILE1 is provided then a matching between T0 and T1 "
								  "is computed and printed to stdout.\n"
								  "If FILE1 is not provided then FILE0 is parsed, T0 is built "
								  "internally and then written to stdout and \"FILE1.replica\".\n"
								  "If ParallelGumtree is called with the -create option, "
								  "pruferTrees is called.";

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
		TreeCreator::pruferTrees(20000, "randomPrufer");
		return 0;

	} else if (argc == 2) {
		// one file was passed, replicate it
		auto file0 = args[1];
		auto t = FileParser::parse(file0);
		cout << t->subTreeToString() << endl;
		FileWriter::write(t, file0 + ".replica");
		delete t; t = nullptr;
		return 0;

	} else if (argc == 3) {
		// two files were passed, match them
		auto file0 = args[1];
		auto file1 = args[2];
		auto t0 = FileParser::parse(file0);
		auto t1 = FileParser::parse(file1);
		// TODO match trees.
		MappingStore mapping; // dummy
		mapping.link(t0, t1); //also dummy
		for (auto m : mapping.asSet())
			cout << m.first->lineNumber() << " -> " << m.second->lineNumber() << endl;
		delete t0; t0 = nullptr;
		delete t1; t1 = nullptr;
		return 0;
	}
	return -1;
}


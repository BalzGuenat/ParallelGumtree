#include <iostream>
#include <fstream>
#include <omp.h>
#include <gperftools/profiler.h>
#include "tree.h"
#include "fileparser.h"
#include "classicgumtree.h"
#include "filewriter.h"
#include "treecreator.h"
#include "mappingstore.h"

using namespace std;

const int DEFAULT_PRUFERTREE_SIZE = 1000;
const string USAGE_TEXT = "ParallelGumtree\n"
								  "Usage: ParallelGumtree [options] [FILE0 FILE1]\n"
								  "\n"
								  "where FILEn encodes a tree Tn.\n"
								  "If FILE0 and FILE1 is provided then a matching between T0 and T1 "
								  "is computed and printed to stdout.\n"
								  "Options:\n"
								  "\t-create [SIZE]\n"
								  "If ParallelGumtree is called with the -create option, "
								  "pruferTrees is called which creates two similar trees "
								  "in the working directory \ncalled randomPrufer1 and randomPrufer2"
								  "with a node count near SIZE.\n"
								  "\t-num-threads N\n"
								  "ParallelGumtree will be executed using N threads.";

int main(int argc, char* argv[])
{
	// make args into proper strings
	vector<string> args(argc);
	for (int i = 0; i < argc; ++i)
		args[i] = argv[i];

	map<string, int> options;

	if (argc < 2) {
		cout << USAGE_TEXT << endl;
		return 0;

	}

	for (int i = 1; i < argc; ++i) {
		auto arg = args[i];
		if (arg[0] == '-')
			options.insert(make_pair(arg,i));
	}

	map<string, int>::iterator idx_it;

	if ((idx_it = options.find("-create")) != options.end()) {
		int i = idx_it->second;
		int size;
		if (i+1 < argc && args[i+1][0] != '-')
			try {
			size = stoi(args[i+1]);
			} catch (const invalid_argument&) {
				size = 0;
			}
		else {
			size = DEFAULT_PRUFERTREE_SIZE;
		}

		if (size < 20) {
			cout << "The SIZE argument provided to the -create option is smaller than 20 or not an integer." << endl;
			return -1;
		}

		TreeCreator::pruferTrees(size, "randomPrufer");
	}

	if ((idx_it = options.find("-num-threads")) != options.end()) {
		int i = idx_it->second;
		int num_threads;
		if (i+1 < argc)
			try {
			num_threads = stoi(args[i+1]);
			omp_set_num_threads(num_threads);
		} catch (const invalid_argument&) {
			cout << "No valid argument to option -num-threads provided." << endl;
			return -1;
		} else {
			cout << "No valid argument to option -num-threads provided." << endl;
			return -1;
		}
	}

	if (argc >= 3) {
		auto file0 = args[argc-2];
		auto file1 = args[argc-1];
		if (file0[0] != '-' && file1[0] != '-') {
			auto t0 = FileParser::parse(file0);
			auto t1 = FileParser::parse(file1);
//			ProfilerStart("/home/guenatb/profile.prof");
			MappingStore mapping;
			ClassicGumtree matcher(t0, t1, &mapping);
			matcher.match();
//			ProfilerStop();
			ofstream outputFile(file1 + "_parallel.mtch");
			for (auto m : mapping.asSet()) {
				//cout << m.first->lineNumber() << " -> " << m.second->lineNumber() << endl;
				outputFile << m.first->lineNumber() << " -> " << m.second->lineNumber() << endl;
			  }
			outputFile.close();
			delete t0; t0 = nullptr;
			delete t1; t1 = nullptr;
		}
	}

	return 0;
}


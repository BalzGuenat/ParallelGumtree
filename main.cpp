#include <iostream>
#include "tree.h"
#include "fileparser.h"
#include "matcher.h"
#include "filewriter.h"
#include "treecreator.h"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 2)
    exit(-1);
  auto t = FileParser::parse(string(argv[1]));
  cout << t->subTreeToString() << endl;
  FileWriter::write(t, "test_tree_out");
  delete t;
  return 0;
}


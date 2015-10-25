#include <iostream>
#include "tree.h"
#include "fileparser.h"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 2)
    exit(-1);
  auto t = FileParser::parse(string(argv[1]));
  cout << t->subTreeToString() << endl;
  delete t;
  return 0;
}


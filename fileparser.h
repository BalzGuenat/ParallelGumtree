#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "tree.h"

class FileParser
{
public:
  static Tree* parse(string filepath);
private:
  static Tree* parseLine(Tree *parent, string line, int lineNumber);
};

#endif // FILEPARSER_H

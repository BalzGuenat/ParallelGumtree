#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>
#include "tree.h"

using namespace std;

class FileWriter
{
public:
  static void write(Tree* tree, string filepath);
};

#endif // FILEWRITER_H

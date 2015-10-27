#include "filewriter.h"
#include <fstream>

void FileWriter::write(Tree *tree, string filepath) {
  ofstream file(filepath);
  file << tree->subTreeToString();
  file.close();
}

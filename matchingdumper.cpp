#include "matchingdumper.h"
#include <fstream>

void MatchingDumper::write(Mapping mapping, string filepath) {
  ofstream file(filepath);
  for (auto match : mapping) {
      file << match.first->lineNumber() << ", ";
      file << match.second->lineNumber() << endl;
    }
  file.close();
}

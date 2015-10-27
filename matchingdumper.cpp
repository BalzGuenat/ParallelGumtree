#include "matchingdumper.h"
#include <fstream>

void MatchingDumper::write(matching matching, string filepath) {
  ofstream file(filepath);
  for (auto match : matching) {
      file << match.first->lineNumber() << ", ";
      file << match.second->lineNumber() << endl;
    }
  file.close();
}

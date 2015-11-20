#include "matchingdumper.h"
#include <fstream>


void MatchingDumper::write(MappingStore mappings, string filepath) {
  ofstream file(filepath);
  for (auto m : mappings) {
		file << m.first->lineNumber() << ", ";
		file << m.second->lineNumber() << endl;
    }
  file.close();
}

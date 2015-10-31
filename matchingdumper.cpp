#include "matchingdumper.h"
#include <fstream>


void MatchingDumper::write(MappingStore mappings, string filepath) {
  ofstream file(filepath);
  for (unordered_map<Tree*, Tree*>::iterator it = mappings.get_iterator_begin(); it != mappings.get_iterator_end(); it ++) {
      file << (*it).first->lineNumber() << ", ";
      file << (*it).second->lineNumber() << endl;
    }
  file.close();
}

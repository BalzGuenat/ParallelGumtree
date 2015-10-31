#ifndef MATCHINGDUMPER_H
#define MATCHINGDUMPER_H

#include <string>
#include "matcher.h"
#include <mappingstore.h>

class MatchingDumper
{
public:
  static void write(MappingStore mapping, string filepath);
};

#endif // MATCHINGDUMPER_H

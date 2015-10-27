#ifndef MATCHINGDUMPER_H
#define MATCHINGDUMPER_H

#include <string>
#include "matcher.h"

class MatchingDumper
{
public:
  static void write(matching matching, string filepath);
};

#endif // MATCHINGDUMPER_H

#ifndef GREEDYSUBTREEMATCHER_H
#define GREEDYSUBTREEMATCHER_H

#include "subtreematcher.h"

class GreedySubTreeMatcher : public SubTreeMatcher
{
public:

GreedySubTreeMatcher(Tree* src, Tree* dst, MappingStore* store);
void filterMappings(MultiMappingStore& multiMappings);
~GreedySubTreeMatcher();

private:
  struct Comparator;
};

#endif // GREEDYSUBTREEMATCHER_H

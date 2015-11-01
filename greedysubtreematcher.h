#ifndef GREEDYSUBTREEMATCHER_H
#define GREEDYSUBTREEMATCHER_H


class GreedySubTreeMatcher : public SubTreeMatcher
{
public:

GreedySubtreeMatcher(Tree* src, Tree* dst, MappingStore* store);
void filterMappings(MultiMappingStore* multiMappings);
~GreedySubTreeMatcher();

private:
  struct comparator {
    bool operator() (int i,int j);
  };
}

#endif // GREEDYSUBTREEMATCHER_H

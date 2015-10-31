#include "greedysubtreematcher.h"


GreedySubTreeMatcher::~GreedySubTreeMatcher()
{

}

GreedySubtreeMatcher::GreedySubtreeMatcher(Tree* src, Tree* dst, MappingStore* store) {
        SubTreeMatcher(src, dst, store);
    }

void GreedySubTreeMatcher::filterMappings(MultiMappingStore *multiMappings) {
        // Select unique mappings first and extract ambiguous mappings.
        vector<Mapping> ambiguousList;
        unordered_set<Tree*> ignored;
        for (set<Tree*>::iterator it = multiMappings.getSrcs().begin(); it != multiMappings.getSrcs().end(); it++) {
            if (multiMappings.isSrcUnique(*it))
                addFullMapping(*it, multiMappings.getDst(*it).begin());
            else if (!ignored.contains(*it)) {
                set<Tree*> adsts = multiMappings.getDst(*it);
                set<Tree*> asrcs = multiMappings.getSrc(multiMappings.getDst(*it).begin());
                for (set<Tree*>::iterator asrc = asrcs.begin(); asrc != asrcs.end(); asrc++)
                    for (set<Tree*>::iterator adst = adsts.begin(); adst != adsts.end(); adst ++)
                        ambiguousList.push_back(Mapping(*asrc, *adst));
                ignored.insert(asrcs.begin, asrcs.end());
            }
        }

        // Rank the mappings by score.
        unordered_set<Tree*> srcIgnored;
        unordered_set<Tree*> dstIgnored;
        sort(ambiguousList.begin(), ambiguousList.end(), compare);

        // Select the best ambiguous mappings
        while (ambiguousList.size() > 0) {
            Mapping ambiguous = ambiguousList.back();
            ambiguousList.pop_back();
            if (!(srcIgnored.contains(ambiguous.first) || dstIgnored.contains(ambiguous.second))) {
                addFullMapping(ambiguous.first, ambiguous.second);
                srcIgnored.insert(ambiguous.first);
                dstIgnored.insert(ambiguous.second);
            }
        }
}

struct GreedySubTreeMatcher::comparator {
  bool operator() (Mapping &i,Mapping &j)
  {

  }
} compare;

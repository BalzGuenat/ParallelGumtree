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

  unordered_map<Mapping*, double> simMap;
  unordered_map<Tree*, vector<Tree*>> srcDescendants;
  unordered_map<Tree*, set<Tree*>> dstDescendants;

  bool operator() (Mapping &i,Mapping &j)
  {
      return simMap[i] < simMap[j];
  }

  MappingComparator(vector<Mapping*>& mappings)
  {
    for (vector<Mapping>::iterator it = mappings.begin(); it != mappings.end(); it++)
      {
        simMap.insert(*it, sim((*it)->first, (*it)->second));

      }
  }

  int numberOfCommonDescendants(Tree* src, Tree* dst) {
    if (srcDescendants.find(src) == srcDescendants.end())
        srcDescendants.insert(src, src->children());
    if (dstDescendants.find(dst) == dstDescendants.end())
        dstDescendants.insert(dst, unoredered_map(dst->children().begin(), dst->children().end()));

    int common = 0;

    for (set<Tree*>::iterator it = srcDescendants[src].begin(); it != srcDescendants.end(); it++) {
        Tree m = mappings.getDst(*it);
        if (m != null && dstDescendants[&dst].find(m) != dstDescendants[&dst].end())
            common++;
    }

    return common;
  }

  double sim(Tree* src, Tree* dst) {
    double jaccard = jaccardSimilarity(src.getParent(), dst.getParent());
    int posSrc = (src.isRoot()) ? 0 : src.getParent().getChildPosition(src);
    int posDst = (dst.isRoot()) ? 0 : dst.getParent().getChildPosition(dst);
    int maxSrcPos =  (src.isRoot()) ? 1 : src.getParent().getChildren().size();
    int maxDstPos =  (dst.isRoot()) ? 1 : dst.getParent().getChildren().size();
    int maxPosDiff = max(maxSrcPos, maxDstPos);
    //TODO
//    double pos = 1D - ((double) Math.abs(posSrc - posDst) / (double) maxPosDiff);
//    double po = 1D - ((double) Math.abs(src.getId() - dst.getId())
//            / (double) GreedySubtreeMatcher.this.getMaxTreeSize());
    return 100 * jaccard + 10 * pos + po;
  }

  double jaccardSimilarity(Tree* src, Tree* dst) {
    double num = (double) numberOfCommonDescendants(src, dst);
    double den = (double) srcDescendants[src].size() + (double) dstDescendants[dst].size() - num;
    return num / den;
  }

};

#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "greedysubtreematcher.h"

struct GreedySubTreeMatcher::Comparator {

	MappingStore* mappings;
	unsigned maxTreeSize;
  unordered_map<Mapping*, double> simMap;
  unordered_map<Tree*, vector<Tree*>> srcDescendants;
  unordered_map<Tree*, set<Tree*>> dstDescendants;

  bool operator() (Mapping* i,Mapping* j)
  {
		return simMap[i] < simMap[j];
  }

  Comparator(vector<Mapping*> mappings, MappingStore* mappingStore, unsigned maxTreeSize)
	  : mappings(mappingStore), maxTreeSize(maxTreeSize)
  {
	 for (auto m : mappings)
		{
		  simMap.insert({m, sim(m->first, m->second)});
		}
  }

  int numberOfCommonDescendants(Tree* src, Tree* dst) {
	 if (srcDescendants.find(src) == srcDescendants.end())
		  srcDescendants.insert({src, src->children()});
	 if (dstDescendants.find(dst) == dstDescendants.end())
	 {
		 set<Tree*> newset(dst->children().begin(), dst->children().end());
		 pair<Tree*, set<Tree*>> pair(dst, newset);
		  dstDescendants.insert(pair);
	 }

	 int common = 0;

	 for (auto t : srcDescendants[src]) {
		  auto m = mappings->get_dst(t);
		  if (m && dstDescendants[dst].find(m) != dstDescendants[dst].end())
				common++;
	 }

	 return common;
  }

  double sim(Tree* src, Tree* dst) {
	 double jaccard = jaccardSimilarity(src->parent(), dst->parent());
	 int posSrc = (src->isRoot()) ? 0 : src->parent()->childPosition(src);
	 int posDst = (dst->isRoot()) ? 0 : dst->parent()->childPosition(dst);
	 int maxSrcPos =  (src->isRoot()) ? 1 : src->parent()->children().size();
	 int maxDstPos =  (dst->isRoot()) ? 1 : dst->parent()->children().size();
	 int maxPosDiff = max(maxSrcPos, maxDstPos);
	 double pos = 1.0 - ((double) abs(posSrc - posDst) / (double) maxPosDiff);
	 double po = 1.0 - ((double) abs(src->id() - dst->id())
				/ (double) maxTreeSize);
	 return 100 * jaccard + 10 * pos + po;
  }

  double jaccardSimilarity(Tree* src, Tree* dst) {
	 double num = (double) numberOfCommonDescendants(src, dst);
	 double den = (double) srcDescendants[src].size() + (double) dstDescendants[dst].size() - num;
	 return num / den;
  }

};

GreedySubTreeMatcher::GreedySubTreeMatcher(Tree* src, Tree* dst, MappingStore* store)
	: SubTreeMatcher(src, dst, store) {}

void GreedySubTreeMatcher::filterMappings(MultiMappingStore& multiMappings) {
        // Select unique mappings first and extract ambiguous mappings.
		  vector<Mapping*> ambiguousList;
        unordered_set<Tree*> ignored;
		  for (auto src : multiMappings.getSrcs()) {
				if (multiMappings.isSrcUnique(src))
					 add_full_mapping(src, *multiMappings.getDst(src).begin());
				else if (ignored.find(src) == ignored.end()) {
					 set<Tree*> adsts = multiMappings.getDst(src);
					 set<Tree*> asrcs = multiMappings.getSrc(*multiMappings.getDst(src).begin());
					 for (auto asrc : asrcs)
						  for (auto adst : adsts)
								ambiguousList.push_back(new Mapping(asrc, adst));
					 ignored.insert(asrcs.begin(), asrcs.end());
            }
        }

        // Rank the mappings by score.
        unordered_set<Tree*> srcIgnored;
        unordered_set<Tree*> dstIgnored;
		  sort(ambiguousList.begin(), ambiguousList.end(),
				 Comparator(ambiguousList, _mappings, max(_src->size(), _dst->size())));

        // Select the best ambiguous mappings
        while (ambiguousList.size() > 0) {
				auto ambiguous = ambiguousList.back();
            ambiguousList.pop_back();
				if (srcIgnored.find(ambiguous->first) == srcIgnored.end() &&
					 dstIgnored.find(ambiguous->second) == dstIgnored.end()) {
					 add_full_mapping(ambiguous->first, ambiguous->second);
					 srcIgnored.insert(ambiguous->first);
					 dstIgnored.insert(ambiguous->second);
            }
        }
}

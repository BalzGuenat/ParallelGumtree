#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "greedysubtreematcher.h"


GreedySubTreeMatcher::~GreedySubTreeMatcher()
{

}

struct GreedySubTreeMatcher::Comparator {

		MappingStore* mappings;
		unsigned maxTreeSize;
		unordered_map<Mapping*, double> simMap;
		unordered_map<const Tree*, vector<const Tree*>> srcDescendants;
		unordered_map<const Tree*, set<const Tree*>> dstDescendants;

		bool operator() (Mapping* i,Mapping* j)
		{
			return simMap[i] < simMap[j]; // TODO make absolutely sure this is the right way around.
		}

		Comparator(vector<Mapping*> mappings, MappingStore* mappingStore, unsigned maxTreeSize)
			: mappings(mappingStore), maxTreeSize(maxTreeSize)
		{
			for (auto m : mappings)
			{
				simMap.insert({m, sim(m->first, m->second)});
			}
		}

		int numberOfCommonDescendants(const Tree* src, const Tree* dst) {
			if (srcDescendants.find(src) == srcDescendants.end())
				srcDescendants.insert({src, src->children()});
			if (dstDescendants.find(dst) == dstDescendants.end())
			{
				set<const Tree*> newset(dst->children().begin(), dst->children().end());
				pair<const Tree*, set<const Tree*>> pair(dst, newset);
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

		double sim(const Tree* src, const Tree* dst) {
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

		double jaccardSimilarity(const Tree* src, const Tree* dst) {
			double num = (double) numberOfCommonDescendants(src, dst);
			double den = (double) srcDescendants[src].size() + (double) dstDescendants[dst].size() - num;
			return num / den;
		}

};

GreedySubTreeMatcher::GreedySubTreeMatcher(const Tree* src, const Tree* dst, MappingStore* store)
	: SubTreeMatcher(src, dst, store) {}

void GreedySubTreeMatcher::filterMappings(MultiMappingStore& multiMappings) {
	// Select unique mappings first and extract ambiguous mappings.
	vector<Mapping*> ambiguousList;
	unordered_set<const Tree*> ignored;
	for (auto src : multiMappings.getSrcs()) {
		if (multiMappings.isSrcUnique(src))
			add_full_mapping(src, *multiMappings.getDst(src).begin());
		else if (ignored.find(src) != ignored.end()) {
			set<const Tree*> adsts = multiMappings.getDst(src);
			set<const Tree*> asrcs = multiMappings.getSrc(*multiMappings.getDst(src).begin());
			for (auto asrc : asrcs)
				for (auto adst : adsts)
					ambiguousList.push_back(new Mapping(asrc, adst));
			ignored.insert(asrcs.begin(), asrcs.end());
		}
	}

	// Rank the mappings by score.
	unordered_set<const Tree*> srcIgnored;
	unordered_set<const Tree*> dstIgnored;
	sort(ambiguousList.begin(), ambiguousList.end(),
		  Comparator(ambiguousList, mappings, max(src->size(), dst->size())));

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

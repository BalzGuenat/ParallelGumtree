#ifndef GREEDYBOTTOMUPMATCHER_H
#define GREEDYBOTTOMUPMATCHER_H

#include "matcher.h"
#include "treemap.h"

class GreedyBottomUpMatcher : public Matcher
{
	public:
		GreedyBottomUpMatcher(Tree* src, Tree* dst, MappingStore* store);
		void match();
	private:
		static const double SIM_THRESHOLD;
		static const unsigned SIZE_THRESHOLD = 1000;

		vector<Tree*> getDstCandidates(Tree* src);
		void lastChanceMatch(Tree* src, Tree* dst);

		TreeMap _srcIds;
		TreeMap _dstIds;

};

inline GreedyBottomUpMatcher::GreedyBottomUpMatcher(Tree *src, Tree *dst, MappingStore *store)
	: Matcher(src, dst, store), _srcIds(src), _dstIds(dst) {}

#endif // GREEDYBOTTOMUPMATCHER_H

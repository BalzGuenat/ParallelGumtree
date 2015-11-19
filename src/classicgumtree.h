#ifndef CLASSICGUMTREE_H
#define CLASSICGUMTREE_H

#include "matcher.h"
#include "mappingstore.h"
#include "greedysubtreematcher.h"
#include "greedybottomupmatcher.h"

class ClassicGumtree : public Matcher
{
	private:
		GreedySubTreeMatcher _topDownMatcher;
		GreedyBottomUpMatcher _bottomUpMatcher;
	public:
		ClassicGumtree(Tree* src, Tree* dst, MappingStore* store);
		void match();

};

#endif // CLASSICGUMTREE_H

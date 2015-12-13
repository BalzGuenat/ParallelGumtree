#include <iostream>
#include "classicgumtree.h"

ClassicGumtree::ClassicGumtree(Tree* src, Tree* dst, MappingStore* store)
	: Matcher(src, dst, store),
	  _topDownMatcher(src, dst, store),
	  _bottomUpMatcher(src, dst, store) {}

void ClassicGumtree::match() {
	auto phase_1_start = clock();
	_topDownMatcher.match();
	auto phase_2_start = clock();
	cout << "First phase: \t" << phase_2_start - phase_1_start << endl;
	_bottomUpMatcher.match();
	auto phase_2_end = clock();
	cout << "Second phase: \t" << phase_2_end - phase_2_start << endl;
}

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
	auto phase_1_mappings = _mappings->size();
	cout << "First phase: \tT = " << phase_2_start - phase_1_start;
	cout << "\tM = " << phase_1_mappings << endl;
	_bottomUpMatcher.match();
	auto phase_2_end = clock();
	auto phase_2_mappings = _mappings->size() - phase_1_mappings;
	cout << "Second phase: \tT = " << phase_2_end - phase_2_start;
	cout << "\tM = " << phase_2_mappings << endl;
}

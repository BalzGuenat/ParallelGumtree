#include "classicgumtree.h"

ClassicGumtree::ClassicGumtree(Tree* src, Tree* dst, MappingStore* store)
	: Matcher(src, dst, store),
	  _topDownMatcher(src, dst, store),
	  _bottomUpMatcher(src, dst, store) {}

void ClassicGumtree::match() {
	_topDownMatcher.match();
	_bottomUpMatcher.match();
}

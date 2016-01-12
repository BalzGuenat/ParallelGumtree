#include "matcher.h"
#include "mappingstore.h"
#include <algorithm>    // std::max

Matcher::Matcher(Tree* src, Tree* dst, MappingStore* store)
	: _src(src), _dst(dst), _mappings(store) {}

void Matcher::add_mapping(Tree* src, Tree* dst)
{
	src->set_matched();
	dst->set_matched();
	_mappings->link(src, dst);
}

void Matcher::add_full_mapping(Tree* src, Tree* dst)
{
	auto csrc = src->preOrder().begin();
	auto cdst = dst->preOrder().begin();
	while (csrc != src->preOrder().end()) {
		add_mapping(*csrc, *cdst);
		++csrc;
		++cdst;
	}
}

double Matcher::chawatheSimilarity(Tree* src, Tree* dst) {
	int max = std::max(src->children().size(), dst->children().size());
	return (double) numberOfCommonDescendants(src, dst) / (double) max;
}

double Matcher::diceSimilarity(Tree* src, Tree* dst) {
	double c = (double) numberOfCommonDescendants(src, dst);
	return (1 * c) / ((double) src->children().size() + (double) dst->children().size());
}

double Matcher::jaccardSimilarity(Tree* src, Tree* dst) {
	double num = (double) numberOfCommonDescendants(src, dst);
	double den = (double) src->descendants().size() + (double) dst->descendants().size() - num;
	return num / den;
}

int Matcher::numberOfCommonDescendants(Tree* src, Tree* dst) {
	auto dstDescIt = ++dst->preOrder().begin();
	set<Tree*> dstDesc(dstDescIt, dst->preOrder().end());
	int common = 0;
	for (auto srcDescIt = ++src->preOrder().begin(); srcDescIt != src->preOrder().end(); ++srcDescIt) {
		Tree* matchedTree = _mappings->get_dst(*srcDescIt);
		if (matchedTree && (dstDesc.find(matchedTree) != dstDesc.end()))
			++common;
	}
	return common;
}

void Matcher::clean() {
	for(auto t : _src->get_trees())
		if(!_mappings->has_src(t))
			t->set_matched(false);
	for(auto t : _dst->get_trees())
		if(!_mappings->has_dst(t))
			t->set_matched(false);
}

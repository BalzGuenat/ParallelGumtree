#include "matcher.h"
#include "mappingstore.h"

#include <set>
#include <algorithm>    // std::max

Matcher::~Matcher()
{

}

Matcher::Matcher(const Tree* src, const Tree* dst, MappingStore* store)
{
	this->src = src;
	this->dst = dst;
	mappings = store;
}

const MappingStore* Matcher::get_mappings()
{
	return mappings;
}

set<Mapping> Matcher::get_mapping_set()
{
	return mappings->asSet();
}

const Tree* Matcher::get_src()
{
	return src;
}

const Tree* Matcher::get_dst()
{
	return dst;
}

void Matcher::add_mapping(const Tree* src, const Tree* dst)
{
	src->setMatched();
	dst->setMatched();
	mappings->link(src, dst);
}

void Matcher::add_full_mapping(const Tree* src, const Tree* dst)
{
	auto csrcs = src->getTrees();
	auto cdsts = dst->getTrees();
	for(unsigned int i = 0; i < csrcs.size(); i++)
	{
		add_mapping(csrcs[i], cdsts[i]);
	}
}

double Matcher::chawatheSimilarity(const Tree* src, const Tree* dst) {
	int max = std::max(src->children().size(), dst->children().size());
	return (double) numberOfCommonDescendants(src, dst) / (double) max;
}

double Matcher::diceSimilarity(const Tree* src, const Tree* dst) {
	double c = (double) numberOfCommonDescendants(src, dst);
	return (1 * c) / ((double) src->children().size() + (double) dst->children().size());
}

double Matcher::jaccardSimilarity(const Tree* src, const Tree* dst) {
	double num = (double) numberOfCommonDescendants(src, dst);
	double den = (double) src->children().size() + (double) dst->children().size() - num;
	return num / den;
}

int Matcher::numberOfCommonDescendants(const Tree* src, const Tree* dst)
{
	auto dstDescVec = dst->descendants();
	set<const Tree*> kids(dstDescVec.begin(), dstDescVec.end());
	int common = 0;
	for (auto c : src->descendants())
	{
		auto match = mappings->get_dst(c);
		if(match && (std::find(kids.begin(), kids.end(), match) != kids.end()))
		{
			common ++;
		}
	}

	return common;
}

void Matcher::clean()
{
	auto srcTrees = src->getTrees();
	for(auto t : srcTrees)
	{
		if(!mappings->has_src(t))
			t->setMatched(false);
	}

	auto dstTrees = dst->getTrees();
	for(auto t : dstTrees)
	{
		if(!mappings->has_dst(t))
			t->setMatched(false);
	}

}

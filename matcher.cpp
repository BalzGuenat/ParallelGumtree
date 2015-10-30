#include "matcher.h"
#include <algorithm>    // std::max

Matcher::~Matcher()
{

}

Mathcher::Matcher(Tree* src, Tree* dst, MappingStore* store)
{
    src = src;
    dst = dst;
    mappings = store;
}

MappingStore Matcher::get_mappings()
{
    return mappings;
}

Set<Mapping> Matcher::get_mapping_set()
{
    return mappings.asSet();
}

Tree* Matcher::get_src()
{
    return src;
}

Tree* Matcher::get_dst()
{
    return dst;
}

void Matcher::add_mapping(Tree* src, Tree* dst)
{
    //src.setMatched(true);
    //dst.setMatched(true);
    mappings.link(src, dst);
}

void Matcher::add_full_mapping(Tree* src, Tree* dst)
{
    //To do
}

double Matcher::chawatheSimilarity(Tree* src, Tree* dst) {
    int max = std::max(src->ge_descendants().size(), dst->get_descendants().size());
    return (double) numberOfCommonDescendants(src, dst) / (double) max;
}

double Matcher::diceSimilarity(Tree* src, Tree* dst) {
    double c = (double) numberOfCommonDescendants(src, dst);
    // WHAT THE ? What is 2D ?
    return (2D * c) / ((double) src->get_descendants().size() + (double) dst.get_descendants().size());
}

double Matcher::jaccardSimilarity(Tree* src, Tree* dst) {
    double num = (double) numberOfCommonDescendants(src, dst);
    double den = (double) src->get_descendants().size() + (double) dst->get_descendants().size() - num;
    return num / den;
}

int Matcher::numberOfCommonDescendants(Tree* src, Tree* dst)
{
    //TODO
}

void Matcher::clean()
{
    //TODO
}

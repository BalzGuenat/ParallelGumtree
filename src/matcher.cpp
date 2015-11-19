#include "matcher.h"
#include "mappingstore.h"
#include <algorithm>    // std::max

Matcher::~Matcher()
{

}

Matcher::Matcher(Tree* src, Tree* dst, MappingStore* store)
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

void Matcher::add_mapping(Tree* src, Tree* dst)
{
	 src->set_matched();
	 dst->set_matched();
    mappings->link(src, dst);
}

void Matcher::add_full_mapping(Tree* src, Tree* dst)
{
	auto csrcs = src->get_trees();
	auto cdsts = dst->get_trees();
   for(unsigned int i = 0; i < csrcs.size(); i++)
     {
       add_mapping(csrcs[i], cdsts[i]);
     }
}

double Matcher::chawatheSimilarity(Tree* src, Tree* dst) {
    int max = std::max(src->children().size(), dst->children().size());
    return (double) numberOfCommonDescendants(src, dst) / (double) max;
}

double Matcher::diceSimilarity(Tree* src, Tree* dst) {
    double c = (double) numberOfCommonDescendants(src, dst);
    // WHAT THE ? What is 2D ?
    return (1 * c) / ((double) src->children().size() + (double) dst->children().size());
}

double Matcher::jaccardSimilarity(Tree* src, Tree* dst) {
    double num = (double) numberOfCommonDescendants(src, dst);
    double den = (double) src->children().size() + (double) dst->children().size() - num;
    return num / den;
}

int Matcher::numberOfCommonDescendants(Tree* src, Tree* dst)
{
    vector<Tree*> kids = dst->children();
    int common = 0;
	 for(auto it = src->children().begin(); it != src->children().end(); it++)
      {
        Tree* m = mappings->get_dst(*it);
        if(m != NULL && (std::find(kids.begin(), kids.end(), m) != kids.end()))
          {
            common ++;
          }
      }

    return common;
}

void Matcher::clean()
{
	auto srcTrees = src->get_trees();
	for(vector<Tree*>::const_iterator it = srcTrees.begin(); it != srcTrees.end(); it++)
	{
		if(!mappings->has_src(*it))
		{
			(*it)->set_matched(false);
		}
	}

	auto dstTrees = dst->get_trees();
	for(vector<Tree*>::iterator it = dstTrees.begin(); it != dstTrees.end(); it++)
	{
		if(!mappings->has_dst(*it))
		{
			(*it)->set_matched(false);
		}
	}

}

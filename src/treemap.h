#ifndef TREEMAP_H
#define TREEMAP_H

#include <map>

#include "tree.h"

class TreeMap : public map<int, Tree*>
{
	public:
		TreeMap(Tree* root);
};

#endif // TREEMAP_H

#include "treemap.h"

TreeMap::TreeMap(Tree* root)
	: map<int, Tree*>()
{
	for (auto t : root->getTrees()) {
		insert({t->id(), t});
	}
}


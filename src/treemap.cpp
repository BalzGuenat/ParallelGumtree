#include "treemap.h"

TreeMap::TreeMap(Tree* root)
	: map<int, Tree*>()
{
	for (auto t : root->preOrder()) {
		insert({t->id(), t});
	}
}


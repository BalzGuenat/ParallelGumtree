#include "treemap.h"

TreeMap::TreeMap(Tree* root)
	: map<int, Tree*>()
{
	for (auto t : root->get_trees()) {
		insert({t->id(), t});
	}
}


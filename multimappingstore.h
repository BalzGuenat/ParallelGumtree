#ifndef MULTIMAPPINGSTORE_H
#define MULTIMAPPINGSTORE_H

#include <map>
#include <set>
#include "tree.h"

class MultiMappingStore
{
	public:
		MultiMappingStore();
		void link(Tree* src, Tree* dst);
		set<Tree*> getSrc(Tree* dst) const;
		set<Tree*> getDst(Tree* src) const;
		set<Tree*> getSrcs() const;
		set<Tree*> getDsts() const;
		bool isSrcUnique(Tree* src) const;
	private:
		multimap<Tree*, Tree*> _srcs;
		multimap<Tree*, Tree*> _dsts;
};

inline void MultiMappingStore::link(Tree *src, Tree *dst) { _srcs.insert({src, dst}); }

#endif // MULTIMAPPINGSTORE_H

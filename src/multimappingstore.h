#ifndef MULTIMAPPINGSTORE_H
#define MULTIMAPPINGSTORE_H

#include <map>
#include <set>
#include "tree.h"

class MultiMappingStore
{
	public:
		MultiMappingStore();
		void link(const Tree* src, const Tree* dst);
		set<const Tree*> getSrc(const Tree* dst) const;
		set<const Tree*> getDst(const Tree* src) const;
		set<const Tree*> getSrcs() const;
		set<const Tree*> getDsts() const;
		bool isSrcUnique(const Tree* src) const;
	private:
		multimap<const Tree*, const Tree*> _srcs;
		multimap<const Tree*, const Tree*> _dsts;
};

inline void MultiMappingStore::link(const Tree *src, const Tree *dst) { _srcs.insert({src, dst}); }

#endif // MULTIMAPPINGSTORE_H

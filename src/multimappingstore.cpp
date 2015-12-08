#include "multimappingstore.h"

MultiMappingStore::MultiMappingStore()
{

}

set<Tree*> MultiMappingStore::getSrc(Tree* dst) const {
	auto r = _dsts.equal_range(dst);
	set<Tree*> src;
	for (auto it = r.first; it != r.second; ++it)
		src.insert(it->second);
	return src;
}

set<Tree*> MultiMappingStore::getDst(Tree* src) const {
	auto r = _srcs.equal_range(src);
	set<Tree*> dst;
	for (auto it = r.first; it != r.second; ++it)
		dst.insert(it->second);
	return dst;
}

set<Tree*> MultiMappingStore::getSrcs() const {
	set<Tree*> srcs;
	for (auto m : _srcs)
		srcs.insert(m.first);
	return srcs;
}

set<Tree*> MultiMappingStore::getDsts() const {
	set<Tree*> dsts;
	for (auto m : _dsts)
		dsts.insert(m.first);
	return dsts;
}

bool MultiMappingStore::isSrcUnique(Tree *src) const {
	auto dst = getDst(src);
	return dst.size() == 1 && getSrc(*dst.begin()).size() == 1;
}

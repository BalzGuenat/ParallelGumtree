#include "multimappingstore.h"

MultiMappingStore::MultiMappingStore()
{

}

set<const Tree*> MultiMappingStore::getSrc(const Tree* dst) const {
	auto r = _dsts.equal_range(dst);
	set<const Tree*> src;
	for (auto it = r.first; it != r.second; ++it)
		src.insert(it->second);
	return src;
}

set<const Tree*> MultiMappingStore::getDst(const Tree* src) const {
	auto r = _dsts.equal_range(src);
	set<const Tree*> dst;
	for (auto it = r.first; it != r.second; ++it)
		dst.insert(it->second);
	return dst;
}

set<const Tree*> MultiMappingStore::getSrcs() const {
	set<const Tree*> srcs;
	for (auto m : _srcs)
		srcs.insert(m.first);
	return srcs;
}

set<const Tree*> MultiMappingStore::getDsts() const {
	set<const Tree*> dsts;
	for (auto m : _dsts)
		dsts.insert(m.first);
	return dsts;
}

bool MultiMappingStore::isSrcUnique(const Tree *src) const {
	auto dst = getDst(src);
	return dst.size() == 1 && getSrc(*dst.begin()).size() == 1;
}

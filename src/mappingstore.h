#ifndef MAPPING_STORE
#define MAPPING_STORE

#include <unordered_map>
#include <set>
#include <utility>
#include "tree.h"
#include "mapping.h"

class MappingStore
{
private:
	 unordered_map<const Tree*, const Tree*> srcs;
	 unordered_map<const Tree*, const Tree*> dsts;
public:
    MappingStore(set<Mapping>);
    MappingStore();
    set<Mapping> asSet();
    MappingStore copy();
	 void link(const Tree *src, const Tree *dst);
	 void unlink(const Tree *src, const Tree *dst);
	 const Tree* first_mapped_src_parent(const Tree *src);
	 const Tree* first_mapped_dst_parent(const Tree *dst);
	 const Tree* get_dst(const Tree* src);
	 const Tree* get_src(const Tree* dst);
	 bool has_src(const Tree* src);
	 bool has_dst(const Tree* dst);
	 bool has(const Tree* src, const Tree* dst);
	 unordered_map<const Tree*, const Tree*>::iterator begin();
	 unordered_map<const Tree*, const Tree*>::iterator end();
};
#endif // MAPPING_STORE

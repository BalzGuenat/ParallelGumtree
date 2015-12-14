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
    unordered_map<Tree*, Tree*> srcs;
    unordered_map<Tree*, Tree*> dsts;
public:
    MappingStore(set<Mapping>);
    MappingStore();
	 unsigned size() const;
    set<Mapping> asSet();
    MappingStore copy();
    void link(Tree *src, Tree *dst);
    void unlink(Tree *src, Tree *dst);
    Tree* first_mapped_src_parent(Tree *src);
    Tree* first_mapped_dst_parent(Tree *dst);
    Tree* get_dst(Tree* src);
    Tree* get_src(Tree* dst);
    bool has_src(Tree* src);
    bool has_dst(Tree* dst);
    bool has(Tree* src, Tree* dst);
	 unordered_map<Tree*, Tree*>::iterator begin();
	 unordered_map<Tree*, Tree*>::iterator end();
};

inline unsigned MappingStore::size() const { return srcs.size(); }

#endif // MAPPING_STORE

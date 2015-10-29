#ifndef MAPPING_STORE
#define MAPPING_STORE

#include <unordered_map>
#include <set>
using Mapping = pair<Tree*, Tree*>;


class MappingStore
{
private:
    unordered_map<Tree* , Tree*> srcs;
    unordered_map<Tree*, Tree*> dsts;
public:
    void MappingStore(Set<Mapping>);
    void MappingStore();
    set<Mapping>* asSet();
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
    unordered_map::iterator get_iterator_begin();
    unordered_map::iterator get_iterator_end();
};
#endif // MAPPING_STORE

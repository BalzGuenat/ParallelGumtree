#ifndef MULTIMAPPINGSTORE_H
#define MULTIMAPPINGSTORE_H


class MultiMappingStore
{

private:
    unordered_map<Tree* , unordered_set<Tree*>> srcs;
    unordered_map<Tree*, unordered_set<Tree*>> dsts;
public:
    MultiMappingStore(set<Mapping>);
    MultiMappingStore();
    set<Mapping>* getMappings();
    void link(Tree *src, Tree *dst);
    void unlink(Tree *src, Tree *dst);
    set<Tree*> getSrcs();
    set<Tree*> getDsts();
    set<Tree*>* get_dst(Tree* src);
    set<Tree*>* get_src(Tree* dst);
    bool has_src(Tree* src);
    bool has_dst(Tree* dst);
    bool has(Tree* src, Tree* dst);
    bool isSrcUnique(Tree* src);
    unordered_map<Tree*, Tree*>::iterator get_iterator_begin();
    unordered_map<Tree*, Tree*>::iterator get_iterator_end();
    ~MultiMappingStore();


};

#endif // MULTIMAPPINGSTORE_H

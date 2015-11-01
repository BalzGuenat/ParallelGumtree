#include "multimappingstore.h"

MultiMappingStore::MultiMappingStore()
{

}

MultiMappingStore::~MultiMappingStore()
{

}

MultiMappingStore::MultiMappingStore(set<Mapping> mappings)
{
  for(set<Mapping>::iterator it = mappings.begin(); it != mappings.end(); it++)
    {
      MappingStore::link((*it).first, (*it).second);
    }
}

unordered_set<Mapping>* MultiMappingStore::getMappings()()
{
  unordered_set<Mapping>* mappings;
  for(unordered_map<Tree* , unordered_set<Tree*>>::iterator it = srcs.begin(); it != srcs.end(); it++)
    {
      for(unordered_set<Tree*>::iterator it2 = srcs[*it].begin(); it2 != srcs[*it].end(); it2++)
        {
          (*mappings).insert(make_pair(*it, *it2));
        }
    }
  return mappings;
}

void MultiMappingStore::link(Tree *src, Tree *dst)
{
  if(srcs.find(src) == srcs.end()) srcs.insert(src, unordered_set<Tree*>);
  MultiMappingStore::srcs.insert(make_pair(src, dst));
  if(dsts.find(src) == dsts.end()) dsts.insert(dst, unordered_set<Tree*>);
  MappingStore::dsts.insert(make_pair(dst,src));
}

void MultiMappingStore::unlink(Tree *src, Tree *dst)
{
  MultiMappingStore::srcs[src].erase(dst);
  MultiMappingStore::dsts[dst].erase(src);
}

set<Tree*> MultiMappingStore::get_srcs()
{
  unordered_set<Tree*> keys;
  for(auto kv : srcs) {
      keys.insert(kv.first);
  }
  return keys;
}

set<Tree*> MultiMappingStore::get_dsts()
{
  unordered_set<Tree*> keys;
  for(auto kv : dsts) {
      keys.insert(kv.first);
  }
  return keys;
}

set<Tree*> MultiMappingStore::get_dst(Tree *src){
  return srcs[src];
}

set<Tree*> MultiMappingStore::get_src(Tree *dst)
{
  return dsts[dst];
}

bool MultiMappingStore::has_src(Tree *src)
{
  return srcs.find(src) != srcs.end();
}

bool MultiMappingStore::has_dst(Tree *dst)
{
  return dsts.find(dst) != dsts.end();
}

bool MultiMappingStore::has(Tree *src, Tree *dst)
{
  return srcs[src].find(dst) != srcs[src].end();
}

bool MultiMappingStore::isSrcUnique(Tree* src)
{
  return srcs[src].size() == 1 && dsts[srcs[src].begin()].size() == 1;
}

unordered_map::iterator MultiMappingStore::get_iterator_begin()
{
 return getMappings().begin();
}

unordered_map::iterator MultiMappingStore::get_iterator_end()
{
 return getMappings().end();
}


#include "mappingstore.h"


MappingStore::MappingStore()
{

}

MappingStore::MappingStore(Set<Mapping> mappings)
{
  for(Set<Mapping>::iterator it = mappings.begin(); it != mappings.end(); it++)
    {
      MappingStore::link((*it).first, (*it).second);
    }
}

set<Mapping>* MappingStore::asSet()
{
  set<Mapping> mappings;
  for(unordered_map<Mapping>::iterator it = srcs.begin(); it != srcs.end(); it++)
    {
      (*mappings).insert(make_pair((*it).first, (*it).second));
    }
  return mappings;
}

MappingStore MappingStore::copy()
{
  return MappingStore::MappingStore(MappingStore::asSet());
}

void MappingStore::link(Tree *src, Tree *dst)
{
  MappingStore::srcs.insert(make_pair(src, dst));
  MappingStore::dsts.insert(make_pair(dst,src));
}

void MappingStore::unlink(Tree *src, Tree *dst)
{
  MappingStore::srcs.erase(src);
  MappingStore::dsts.erase(dst);
}

Tree* MappingStore::first_mapped_src_parent(Tree *src)
{
  Tree *parent = (*src).parent();
  if(parent == null) return null;
  else{
      while(!has_src(parent)){
          parent = (*parent).parent;
          if(parent == null) return null;
        }
      return parent;
    }
}

Tree* MappingStore::first_mapped_dst_parent(Tree *dst)
{
  Tree *parent = (*dst).parent();
  if(parent == null) return null;
  else{
      while(!has_dst(parent)){
          parent = (*parent).parent;
          if(parent == null) return null;
        }
      return parent;
    }
}

Tree* MappingStore::get_dst(Tree *src){
  return srcs(src);
}

Tree* MappingStore::get_src(Tree *dst)
{
  return dsts(dst);
}

bool MappingStore::has_src(Tree *src)
{

}

bool MappingStore::has_dst(Tree *dst)
{

}

bool MappingStore::has(Tree *src, Tree *dst)
{
  return srcs(src) == dst;
}

unordered_map::iterator MappingStore::get_iterator_begin()
{
 return srcs.begin();
}

unordered_map::iterator MappingStore::get_iterator_end()
{
 return srcs.end();
}

MappingStore::~MappingStore()
{
//What's that?
}

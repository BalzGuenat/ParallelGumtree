#include "mappingstore.h"


MappingStore::MappingStore() {}

MappingStore::MappingStore(set<Mapping> mappings)
{
  for(auto m : mappings)
    {
      link(m.first, m.second);
    }
}

set<Mapping> MappingStore::asSet()
{
  set<Mapping> mappings;
  for (auto m : srcs)
    {
      mappings.insert(make_pair(m.first, m.second));
    }
  return mappings;
}

MappingStore MappingStore::copy()
{
  return MappingStore(asSet());
}

void MappingStore::link(Tree *src, Tree *dst)
{
  MappingStore::srcs.insert(make_pair(src, dst));
  MappingStore::dsts.insert(make_pair(dst, src));
}

void MappingStore::unlink(Tree *src, Tree *dst)
{
  MappingStore::srcs.erase(src);
  MappingStore::dsts.erase(dst);
}

Tree* MappingStore::first_mapped_src_parent(Tree *src)
{
  auto parent = src->parent();
  if (!parent)
    return nullptr;
  else {
      while (!has_src(parent)) {
          parent = parent->parent();
          if (!parent)
            return nullptr;
        }
      return parent;
    }
}

Tree* MappingStore::first_mapped_dst_parent(Tree *dst)
{
  auto parent = dst->parent();
  if (!parent)
    return nullptr;
  else {
      while (!has_dst(parent)) {
          parent = parent->parent();
          if (!parent)
            return nullptr;
        }
      return parent;
    }
}

Tree* MappingStore::get_dst(Tree *src) {
	auto result = srcs.find(src);
	if (result != srcs.end()) {
		return result->second;
	} else {
		return nullptr;
	}
}

Tree* MappingStore::get_src(Tree *dst) {
	auto result = dsts.find(dst);
	if (result != dsts.end()) {
		return result->second;
	} else {
		return nullptr;
	}
}

bool MappingStore::has_src(Tree *src)
{
	return srcs.find(src) != srcs.end();
}

bool MappingStore::has_dst(Tree *dst)
{
	return dsts.find(dst) != dsts.end();
}

bool MappingStore::has(Tree *src, Tree *dst)
{
  try {
    return srcs.at(src) == dst;
  } catch (out_of_range) {
    return false;
  }
}

unordered_map<Tree*, Tree*>::iterator MappingStore::begin()
{
 return srcs.begin();
}

unordered_map<Tree*, Tree*>::iterator MappingStore::end()
{
 return srcs.end();
}
/*
MappingStore::~MappingStore()
{
//What's that?
  // This is a destructor. It gets called when the object is deallocated/deleted.
  // It's purpose is to delete/free all resources this object owns.
  // Currently, leaving it empty is fine.
}
*/

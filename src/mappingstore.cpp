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

void MappingStore::link(const Tree *src, const Tree *dst)
{
  MappingStore::srcs.insert(make_pair(src, dst));
  MappingStore::dsts.insert(make_pair(dst, src));
}

void MappingStore::unlink(const Tree *src, const Tree *dst)
{
  MappingStore::srcs.erase(src);
  MappingStore::dsts.erase(dst);
}

const Tree* MappingStore::first_mapped_src_parent(const Tree *src)
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

const Tree* MappingStore::first_mapped_dst_parent(const Tree *dst)
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

const Tree* MappingStore::get_dst(const Tree *src){
	try {
		return srcs.at(src);
	} catch (out_of_range) {
		return nullptr;
	}
}

const Tree* MappingStore::get_src(const Tree *dst)
{
	try {
		return dsts.at(dst);
	} catch (out_of_range) {
		return nullptr;
	}
}

bool MappingStore::has_src(const Tree *src)
{
  try {
    srcs.at(src);
    return true;
  } catch (out_of_range) {
      return false;
  }
}

bool MappingStore::has_dst(const Tree *dst)
{
  try {
    dsts.at(dst);
    return true;
  } catch (out_of_range) {
      return false;
  }
}

bool MappingStore::has(const Tree *src, const Tree *dst)
{
  try {
    return srcs.at(src) == dst;
  } catch (out_of_range) {
    return false;
  }
}

unordered_map<const Tree*, const Tree*>::iterator MappingStore::begin()
{
 return srcs.begin();
}

unordered_map<const Tree*, const Tree*>::iterator MappingStore::end()
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

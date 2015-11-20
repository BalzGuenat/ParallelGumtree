#ifndef MATCHER_H
#define MATCHER_H

#include "mappingstore.h"

class Matcher
{

protected:
  Tree* _src;
  Tree* _dst;
  MappingStore* _mappings;

  void add_mapping(Tree* src, Tree* dst);
  void add_full_mapping(Tree* src, Tree* dst);
  double chawatheSimilarity(Tree* src, Tree* dst);
  double diceSimilarity(Tree* src, Tree* dst);
  double jaccardSimilarity(Tree* src, Tree* dst);
  int numberOfCommonDescendants(Tree* src, Tree* dst);
  void clean();

public:
  Matcher(Tree* src, Tree* dst, MappingStore* store);
  virtual void match() = 0;
  const MappingStore* get_mappings();
  set<Mapping> get_mapping_set();
  const Tree* get_src();
  const Tree* get_dst();
};

inline const MappingStore* Matcher::get_mappings() { return _mappings; }
inline set<Mapping> Matcher::get_mapping_set() { return _mappings->asSet(); }
inline const Tree* Matcher::get_src() { return _src; }
inline const Tree* Matcher::get_dst() { return _dst; }

#endif // MATCHER_H

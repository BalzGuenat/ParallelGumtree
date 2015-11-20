#ifndef MATCHER_H
#define MATCHER_H

#include "mappingstore.h"

class Matcher
{

protected:
  const Tree* src;
  const Tree* dst;
  MappingStore* mappings;

  void add_mapping(const Tree* src, const Tree* dst);
  void add_full_mapping(const Tree* src, const Tree* dst);
  double chawatheSimilarity(const Tree* src, const Tree* dst);
  double diceSimilarity(const Tree* src, const Tree* dst);
  double jaccardSimilarity(const Tree* src, const Tree* dst);
  int numberOfCommonDescendants(const Tree* src, const Tree* dst);
  void clean();

public:
  Matcher(const Tree* src, const Tree* dst, MappingStore* store);
  virtual void match() = 0;
  const MappingStore* get_mappings();
  set<Mapping> get_mapping_set();
  const Tree* get_src();
  const Tree* get_dst();

  ~Matcher();
};

#endif // MATCHER_H

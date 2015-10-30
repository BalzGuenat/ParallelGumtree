#ifndef MATCHER_H
#define MATCHER_H

#include <mappingstore.h>

class Matcher
{

protected:
  const Tree* src;
  const Tree* dst;
  const MappingStore* mappings;

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
  MappingStore get_mappings();
  Set<Mapping> get_mapping_set();
  Tree* get_src();
  Tree* get_dst();

  ~Matcher();
};

#endif // MATCHER_H

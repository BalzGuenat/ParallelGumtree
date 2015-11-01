#ifndef SUBTREEMATCHER_H
#define SUBTREEMATCHER_H

#include "matcher.h"

class MultiMappingStore; // TODO define this class.

class SubTreeMatcher : public Matcher
{
protected:

  double sim(Tree* src, Tree* dst);
  int getMaxTreeSize();

public:
  SubTreeMatcher(Tree* src, Tree* dst, MappingStore* store);
  ~SubTreeMatcher();
  void match();
  virtual void filterMappings(MultiMappingStore* mmappings) = 0;


//private:
  class PriorityTreeList
  {
  private:
    vector<Tree*> trees;
    int maxHeight;
    int currentIdx;
    int idx(Tree* tree);
    int idx(int height);
    int height(int idx);
    void addTree(Tree* tree);

  public:
    PriorityTreeList(Tree* tree);
    vector<Tree*> open();
    vector<Tree*> pop();
    void open(Tree* tree);
    int peekHeight();
    void updateHeight();

  };
private:
  void popLarger(PriorityTreeList &srcs, PriorityTreeList &dsts);
  static int MIN_HEIGHT;//TODO

};

#endif // SUBTREEMATCHER_H

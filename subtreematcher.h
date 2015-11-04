#ifndef SUBTREEMATCHER_H
#define SUBTREEMATCHER_H

#include "matcher.h"
#include "multimappingstore.h"

class SubTreeMatcher : public Matcher
{
protected:

  double sim(Tree* src, Tree* dst);
  int getMaxTreeSize();

public:
  SubTreeMatcher(Tree* src, Tree* dst, MappingStore* store);
  ~SubTreeMatcher();
  void match();
  virtual void filterMappings(MultiMappingStore& mmappings) = 0;


//private:
  class PriorityTreeList
  {
  private:
	 vector<vector<Tree*>*> trees;
	 unsigned maxHeight;
	 int currentIdx;
	 unsigned idx(Tree* tree) const;
	 unsigned idx(unsigned height) const;
	 unsigned height(unsigned idx) const;
    void addTree(Tree* tree);

  public:
    PriorityTreeList(Tree* tree);
	 vector<Tree*>* open();
	 vector<Tree*>* pop();
    void open(Tree* tree);
    int peekHeight();
    void updateHeight();

  };

  void popLarger(PriorityTreeList &srcs, PriorityTreeList &dsts);
  static unsigned MIN_HEIGHT;

};

inline unsigned SubTreeMatcher::PriorityTreeList::idx(Tree *tree) const { return idx(tree->height()); }
inline unsigned SubTreeMatcher::PriorityTreeList::idx(unsigned height) const { return maxHeight - height; }
inline unsigned SubTreeMatcher::PriorityTreeList::height(unsigned idx) const { return maxHeight - idx; }

#endif // SUBTREEMATCHER_H

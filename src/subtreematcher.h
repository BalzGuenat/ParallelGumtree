#ifndef SUBTREEMATCHER_H
#define SUBTREEMATCHER_H

#include "matcher.h"
#include "multimappingstore.h"

class SubTreeMatcher : public Matcher
{
protected:

  double sim(const Tree* src, const Tree* dst);
  int getMaxTreeSize();

public:
  SubTreeMatcher(const Tree* src, const Tree* dst, MappingStore* store);
  ~SubTreeMatcher();
  void match();
  virtual void filterMappings(MultiMappingStore& mmappings) = 0;


//private:
  class PriorityTreeList
  {
  private:
	 vector<vector<const Tree*>*> trees;
	 unsigned maxHeight;
	 int currentIdx = 0;
	 unsigned idx(const Tree* tree) const;
	 unsigned idx(unsigned height) const;
	 unsigned height(unsigned idx) const;
	 void addTree(const Tree* tree);

  public:
	 PriorityTreeList(const Tree* tree);
	 vector<const Tree*>* open();
	 vector<const Tree*>* pop();
	 void open(const Tree* tree);
    int peekHeight();
    void updateHeight();

  };

  void popLarger(PriorityTreeList &srcs, PriorityTreeList &dsts);
  static unsigned MIN_HEIGHT;

};

inline unsigned SubTreeMatcher::PriorityTreeList::idx(const Tree *tree) const { return idx(tree->height()); }
inline unsigned SubTreeMatcher::PriorityTreeList::idx(unsigned height) const { return maxHeight - height; }
inline unsigned SubTreeMatcher::PriorityTreeList::height(unsigned idx) const { return maxHeight - idx; }

#endif // SUBTREEMATCHER_H

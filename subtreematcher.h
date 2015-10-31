#ifndef SUBTREEMATCHER_H
#define SUBTREEMATCHER_H


class SubTreeMatcher : public Matcher
{
protected:

  double sim(ITree src, ITree dst);
  int getMaxTreeSize();

public:
  SubtreeMatcher(ITree src, ITree dst, MappingStore store);
  void match();
  virtual void filterMappings(MultiMappingStore mmappings) = 0;

  ~SubTreeMatcher();

private:
  static int MIN_HEIGHT;//TODO
  void popLarger(PriorityTreeList srcs, PriorityTreeList dsts);

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

};

#endif // SUBTREEMATCHER_H

#include "subtreematcher.h"


SubTreeMatcher::SubTreeMatcher()
{

}



SubTreeMatcher::SubtreeMatcher(Tree* src, Tree* dst, MappingStore* store)
{
        Matcher(src, dst, store);
}

void SubTreeMatcher::popLarger(PriorityTreeList* srcs, PriorityTreeList* dsts)
{
        if (srcs.peekHeight() > dsts.peekHeight())
            srcs.open();
        else
            dsts.open();
}

void SubTreeMatcher::match()
{
        MultiMappingStore multiMappings;

        PriorityTreeList srcs(src);
        PriorityTreeList dsts(dst);

        while (srcs.peekHeight() != -1 && dsts.peekHeight() != -1) {
            while (srcs.peekHeight() != dsts.peekHeight())
                popLarger(srcs, dsts);

            vector<Tree*> hSrcs = srcs.pop();
            vector<Tree*> hDsts = dsts.pop();

            vector<bool> srcMarks(hSrcs.size());
            vector<bool> dstMarks(hDsts.size());

            for (int i = 0; i < hSrcs.size(); i++) {
                for (int j = 0; j < hDsts.size(); j++) {
                    Tree* src = hSrcs.get(i);
                    Tree* dst = hDsts.get(j);

                    if (src->isClone(dst)) {
                        multiMappings.link(src, dst);
                        srcMarks[i] = true;
                        dstMarks[j] = true;
                    }
                }
            }

            for (int i = 0; i < srcMarks.length; i++)
                if (srcMarks[i] == false)
                    srcs.open(hSrcs.get(i));
            for (int j = 0; j < dstMarks.length; j++)
                if (dstMarks[j] == false)
                    dsts.open(hDsts.get(j));
            srcs.updateHeight();
            dsts.updateHeight();
        }

        filterMappings(multiMappings);
}

virtual void SubTreeMatcher::filterMappings(MultiMappingStore* mmappings);

double SubTreeMatcher::sim(Tree* src, Tree* dst)
{
  double jaccard = jaccardSimilarity(src.getParent(), dst.getParent());
  int posSrc = (src.isRoot()) ? 0 : src.getParent().getChildPosition(src);
  int posDst = (dst.isRoot()) ? 0 : dst.getParent().getChildPosition(dst);
  int maxSrcPos =  (src.isRoot()) ? 1 : src.getParent().getChildren().size();
  int maxDstPos =  (dst.isRoot()) ? 1 : dst.getParent().getChildren().size();
  int maxPosDiff = max(maxSrcPos, maxDstPos);
  //TODO
  double pos = 1D - ((double) abs(posSrc - posDst) / (double) maxPosDiff);
  double po = 1D - ((double) abs(src.getId() - dst.getId()) / (double) this.getMaxTreeSize());
  return 100 * jaccard + 10 * pos + po;
}

int SubTreeMatcher::getMaxTreeSize()
{
  return max(src.getSize(), dst.getSize());
}


SubTreeMatcher::~SubTreeMatcher()
{

}


#include "subtreematcher.h"

unsigned SubTreeMatcher::MIN_HEIGHT = 2;

SubTreeMatcher::SubTreeMatcher(Tree* src, Tree* dst, MappingStore* store)
  : Matcher(src, dst, store) {}

void SubTreeMatcher::popLarger(PriorityTreeList& srcs, PriorityTreeList& dsts)
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

				auto hSrcs = srcs.pop();
				auto hDsts = dsts.pop();

				vector<bool> srcMarks(hSrcs->size());
				vector<bool> dstMarks(hDsts->size());

				for (unsigned i = 0; i < hSrcs->size(); i++) {
					 for (unsigned j = 0; j < hDsts->size(); j++) {
						  Tree* src = hSrcs->at(i);
						  Tree* dst = hDsts->at(j);

                    if (src->isClone(dst)) {
                        multiMappings.link(src, dst);
                        srcMarks[i] = true;
                        dstMarks[j] = true;
                    }
                }
            }

            for (unsigned i = 0; i < srcMarks.size(); i++)
                if (srcMarks[i] == false)
						  srcs.open(hSrcs->at(i));
            for (unsigned j = 0; j < dstMarks.size(); j++)
                if (dstMarks[j] == false)
						  dsts.open(hDsts->at(j));
            srcs.updateHeight();
            dsts.updateHeight();
        }

        filterMappings(multiMappings);
}

double SubTreeMatcher::sim(Tree* src, Tree* dst)
{
  double jaccard = jaccardSimilarity(src->parent(), dst->parent());
  int posSrc = (src->isRoot()) ? 0 : src->parent()->childPosition(src);
  int posDst = (dst->isRoot()) ? 0 : dst->parent()->childPosition(dst);
  int maxSrcPos =  (src->isRoot()) ? 1 : src->parent()->children().size();
  int maxDstPos =  (dst->isRoot()) ? 1 : dst->parent()->children().size();
  int maxPosDiff = max(maxSrcPos, maxDstPos);
  //TODO
  double pos = 1.0 - ((double) abs(posSrc - posDst) / (double) maxPosDiff);
  // We don't have numbering yet so don't use po for now. It's only a small factor anyways.
  //double po = 1.0 - ((double) abs(src->id() - dst.getId()) / (double) this.getMaxTreeSize());
  return 100 * jaccard + 10 * pos/* + po*/;
}

int SubTreeMatcher::getMaxTreeSize()
{
  return max(src->size(), dst->size());
}


SubTreeMatcher::~SubTreeMatcher()
{

}


int SubTreeMatcher::PriorityTreeList::peekHeight() {
	return (currentIdx == -1) ? -1 : height(currentIdx);
}

void SubTreeMatcher::PriorityTreeList::open(Tree *tree) {
	for (auto child : tree->children())
		addTree(child);
}

void SubTreeMatcher::PriorityTreeList::updateHeight() {
	currentIdx = -1;
	for (unsigned i = 0; i < trees.size(); i++) {
		 if (trees[i]) {
			  currentIdx = i;
			  break;
		 }
	}
}

vector<Tree*>* SubTreeMatcher::PriorityTreeList::pop() {
	if (currentIdx == -1)
		return nullptr;
	else {
		auto pop = trees[currentIdx];
		trees[currentIdx] = nullptr;
		return pop;
	}
}

void SubTreeMatcher::PriorityTreeList::addTree(Tree *tree) {
	if (tree->height() >= MIN_HEIGHT) {
		int i = idx(tree);
		if (!trees.at(i))
			trees.at(i) = new vector<Tree*>;
		trees.at(i)->push_back(tree);
	}
}

vector<Tree*>* SubTreeMatcher::PriorityTreeList::open() {
	auto popped = pop();
	if (popped) {
		for (auto tree : *popped)
			open(tree);
		updateHeight();
		return popped;
	} else
		return nullptr;
}

SubTreeMatcher::PriorityTreeList::PriorityTreeList(Tree *tree) {
	int listSize = tree->height() - MIN_HEIGHT + 1;
	if (listSize < 0)
		listSize = 0;
	if (listSize == 0)
		currentIdx = -1;
	trees = vector<vector<Tree*>*>(listSize);
	maxHeight = tree->height();
	addTree(tree);
}





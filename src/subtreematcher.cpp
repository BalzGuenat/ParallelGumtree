#include "subtreematcher.h"
#include <iostream>
#include <map>

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

	PriorityTreeList srcs(_src);
	PriorityTreeList dsts(_dst);

	while (srcs.peekHeight() != -1 && dsts.peekHeight() != -1) {
		while (srcs.peekHeight() != dsts.peekHeight())
			popLarger(srcs, dsts);

		//cout << "Height: " << srcs.peekHeight() << endl;

		auto hSrcs = srcs.pop();

		multimap<unsigned, Tree*> hDstsMap;
		auto hDsts = dsts.pop();
		for (auto dst : *hDsts) {
			hDstsMap.insert(make_pair(dst->hash(), dst));
		}
		delete hDsts;

		vector<bool> srcMarks(hSrcs->size());
		//vector<bool> dstMarks(hDsts->size());

		//cout << "Checking " << hSrcs->size() * hDsts->size() << " pairs." << endl;

		// TODO smartly prallelize these loops
		// suggestion: each task produces pairs to be linked. in the end, these sets of pairs are collected and by a single thread added to the mapping store.
		int counter = 0;
		const auto hSrcsSize = hSrcs->size();
//#pragma omp parallel for
		for (unsigned i = 0; i < hSrcsSize; i++) {
			Tree* src = hSrcs->at(i);
			auto range = hDstsMap.equal_range(src->hash());
			for (auto dstIt = range.first; dstIt != range.second;) {
				++counter;
				Tree* dst = dstIt->second;
				if (src->isClone(dst)) {
					multiMappings.link(src, dst);
					srcMarks[i] = true;
					auto toDelete = dstIt++;
					hDstsMap.erase(toDelete);
				} else
					++dstIt;
			}
		}

		//cout << "Checked " << counter << " pairs." << endl;

		for (unsigned i = 0; i < srcMarks.size(); i++)
			if (srcMarks[i] == false)
				srcs.open(hSrcs->at(i));
		for (auto dstPair : hDstsMap)
			dsts.open(dstPair.second);
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
  double pos = 1.0 - ((double) abs(posSrc - posDst) / (double) maxPosDiff);
  double po = 1.0 - ((double) abs(src->id() - dst->id()) /
							(double) getMaxTreeSize());
  return 100 * jaccard + 10 * pos + po;
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





#include <vector>
#include <set>

#include "greedybottomupmatcher.h"
#include "treemap.h"

const double GreedyBottomUpMatcher::SIM_THRESHOLD = 0.5;

void GreedyBottomUpMatcher::match() {
	_srcIds = TreeMap(_src);
	_dstIds = TreeMap(_dst);

	for (Tree* t : _src->postOrder()) {
		if (t->isRoot()) {
			add_mapping(t, _dst);
			lastChanceMatch(t, _dst);
			break;
		} else if (!(t->isMatched() || t->isLeaf())) {
			auto candidates = getDstCandidates(t);
			Tree* best = nullptr;
			double max = -1.0;

			// TODO parallelize this loop
			for (auto c : candidates) {
				double sim = jaccardSimilarity(t, c);
				if (sim > max && sim >= SIM_THRESHOLD) {
					best = c;
					max = sim;
				}
			}

			if (best) {
				lastChanceMatch(t, best);
				add_mapping(t, best);
			}
		}
	}
	clean();
}

vector<Tree*> GreedyBottomUpMatcher::getDstCandidates(Tree* src) {
	vector<Tree*> seeds;
	for (auto c : src->children()) {
		auto m = _mappings->get_dst(c);
		if (m)
			seeds.push_back(m);
	}
	vector<Tree*> candidates;
	set<Tree*> visited;
	for (Tree* seed : seeds) {
		while (seed->parent()) {
			auto parent = seed->parent();
			if (visited.find(parent) != visited.cend())
				break;
			visited.insert(parent);
			if (parent->type() == src->type() && !parent->isMatched() && !parent->isRoot())
				candidates.push_back(parent);
			seed = parent;
		}
	}
	return candidates;
}

void GreedyBottomUpMatcher::lastChanceMatch(Tree* src, Tree* dst) {
	// not properly implemented because we have no ZsMatcher.

	for (auto t : src->get_trees())
		t->set_matched();
	for (auto t : dst->get_trees())
		t->set_matched();
}

#include <vector>
#include <set>
#include <utility>
#include <iostream>

#include "greedybottomupmatcher.h"
#include "treemap.h"

const double GreedyBottomUpMatcher::SIM_THRESHOLD = 0.5;

const unsigned MIN_CANDIDATES_SIZE_FOR_PARALLEL = 1 << 3;

#pragma omp declare reduction(pair_max : pair<double, Tree*> : \
	omp_out = max(omp_in, omp_out)) \
	initializer(omp_priv=make_pair(-1.0, nullptr))

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
			pair<double,Tree*> maxPair = make_pair(-1.0, nullptr);
			unsigned candidatesSize = candidates.size();

#pragma omp parallel for reduction(pair_max:maxPair) if(candidatesSize > MIN_CANDIDATES_SIZE_FOR_PARALLEL)
			for (unsigned i = 0; i < candidatesSize; ++i) {
				auto c = candidates[i];
				double sim = jaccardSimilarity(t, c);
				if (sim > maxPair.first && sim >= SIM_THRESHOLD) {
					maxPair = make_pair(sim, c);
				}
			}

			auto best = maxPair.second;
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
	for (auto c : src->descendants()) {
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

	for (auto t : src->preOrder())
		t->set_matched();
	for (auto t : dst->preOrder())
		t->set_matched();
}

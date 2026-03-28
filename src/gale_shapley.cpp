#include "gale_shapley.h"
#include <cassert>
#include <queue>

GaleShapley::GaleShapley(const std::vector<std::vector<int>>& groupAPrefs,
                         const std::vector<std::vector<int>>& groupBPrefs)
    : n_(static_cast<int>(groupAPrefs.size())),
      prefA_(groupAPrefs)
{
    assert(static_cast<int>(groupBPrefs.size()) == n_);

    // Pre-compute rank tables for group A so isStable() checks are O(1).
    rankA_.assign(n_, std::vector<int>(n_, 0));
    for (int i = 0; i < n_; ++i) {
        assert(static_cast<int>(groupAPrefs[i].size()) == n_);
        for (int rank = 0; rank < n_; ++rank) {
            rankA_[i][groupAPrefs[i][rank]] = rank;
        }
    }

    // Pre-compute rank tables for group B so acceptability checks are O(1).
    rankB_.assign(n_, std::vector<int>(n_, 0));
    for (int j = 0; j < n_; ++j) {
        assert(static_cast<int>(groupBPrefs[j].size()) == n_);
        for (int rank = 0; rank < n_; ++rank) {
            rankB_[j][groupBPrefs[j][rank]] = rank;
        }
    }
}

void GaleShapley::run() {
    matchA_.assign(n_, -1);
    matchB_.assign(n_, -1);

    // next_[i] = index into prefA_[i] of the next person A-person i will propose to.
    std::vector<int> next(n_, 0);

    // Queue of unmatched A-side proposers.
    std::queue<int> free;
    for (int i = 0; i < n_; ++i) free.push(i);

    while (!free.empty()) {
        int a = free.front();
        free.pop();

        // Propose to next preferred B-person.
        int b = prefA_[a][next[a]++];

        if (matchB_[b] == -1) {
            // b is free — tentatively match.
            matchA_[a] = b;
            matchB_[b] = a;
        } else {
            int current = matchB_[b];
            if (rankB_[b][a] < rankB_[b][current]) {
                // b prefers a — swap.
                matchA_[a] = b;
                matchB_[b] = a;
                matchA_[current] = -1;
                free.push(current);
            } else {
                // b prefers current — a remains free.
                free.push(a);
            }
        }
    }
}

bool GaleShapley::isStable() const {
    assert(!matchA_.empty() && "isStable() called before run()");

    // For every (a, b) pair that isn't matched, check that neither would both
    // prefer each other over their current partners.
    for (int a = 0; a < n_; ++a) {
        if (matchA_[a] == -1) continue;  // a is unmatched — skip
        for (int b = 0; b < n_; ++b) {
            if (matchA_[a] == b) continue;
            if (matchB_[b] == -1) continue;  // b is unmatched — can't form a blocking pair

            // Does a prefer b over matchA_[a]?  (O(1) via rank table)
            if (rankA_[a][b] >= rankA_[a][matchA_[a]]) continue;

            // Does b prefer a over matchB_[b]?
            if (rankB_[b][a] < rankB_[b][matchB_[b]]) return false;  // blocking pair found
        }
    }
    return true;
}

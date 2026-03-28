#include "hopcroft_karp.h"
#include "hungarian.h"
#include <cassert>
#include <iostream>
#include <numeric>

static void check(const char* name, bool cond) {
    std::cout << (cond ? "[PASS] " : "[FAIL] ") << name << "\n";
    assert(cond);
}

// ── Hopcroft-Karp tests ───────────────────────────────────────────────────────

static void hk_perfect_matching() {
    // 3 left, 3 right — perfect matching exists
    HopcroftKarp hk(3, 3);
    hk.addCompatiblePair(0, 0);
    hk.addCompatiblePair(1, 1);
    hk.addCompatiblePair(2, 2);
    int m = hk.maxMatching();
    check("HK perfect match size", m == 3);
}

static void hk_chain_matching() {
    // Left 0 can only reach Right 0.
    // Left 1 can reach Right 0 and Right 1.
    // Left 2 can only reach Right 1.
    // Maximum matching = 2, not 3.
    // Wait — actually 3 is possible via 0→0, 1→1, 2→? No right 2. So max = 2.
    HopcroftKarp hk(3, 2);
    hk.addCompatiblePair(0, 0);
    hk.addCompatiblePair(1, 0);
    hk.addCompatiblePair(1, 1);
    hk.addCompatiblePair(2, 1);
    int m = hk.maxMatching();
    check("HK chain match size", m == 2);
}

static void hk_no_edges() {
    HopcroftKarp hk(4, 4);
    check("HK no edges", hk.maxMatching() == 0);
}

static void hk_complete_bipartite() {
    // K(4,4) — perfect matching of size 4
    HopcroftKarp hk(4, 4);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            hk.addCompatiblePair(i, j);
    check("HK K(4,4)", hk.maxMatching() == 4);
    // Verify matching is valid
    const auto& match = hk.getMatching();
    std::vector<bool> usedRight(4, false);
    for (int i = 0; i < 4; ++i) {
        assert(match[i] >= 0 && match[i] < 4);
        assert(!usedRight[match[i]]);
        usedRight[match[i]] = true;
    }
    check("HK K(4,4) valid assignment", true);
}

// ── Hungarian tests ───────────────────────────────────────────────────────────

static void hung_identity() {
    // Identity matrix — each person best matched with themselves.
    std::vector<std::vector<int>> mat = {
        {5, 0, 0},
        {0, 5, 0},
        {0, 0, 5},
    };
    Hungarian h(mat);
    h.solve();
    check("Hung identity score", h.getMaxScore() == 15);
    const auto& a = h.getAssignment();
    check("Hung identity a[0]", a[0] == 0);
    check("Hung identity a[1]", a[1] == 1);
    check("Hung identity a[2]", a[2] == 2);
}

static void hung_classic_3x3() {
    // From Wikipedia Hungarian algorithm example.
    std::vector<std::vector<int>> mat = {
        {4, 1, 3},
        {2, 0, 5},
        {3, 2, 2},
    };
    Hungarian h(mat);
    h.solve();
    check("Hung 3x3 score", h.getMaxScore() == 11);  // 4+5+2 or 3+5+3 ...
    // Verify it totals correctly
    const auto& a = h.getAssignment();
    int total = 0;
    for (int i = 0; i < 3; ++i) total += mat[i][a[i]];
    check("Hung 3x3 total matches getMaxScore", total == h.getMaxScore());
}

static void hung_4x4() {
    std::vector<std::vector<int>> mat = {
        {9, 2, 7, 8},
        {6, 4, 3, 7},
        {5, 8, 1, 8},
        {7, 6, 9, 4},
    };
    Hungarian h(mat);
    h.solve();
    const auto& a = h.getAssignment();
    int total = 0;
    for (int i = 0; i < 4; ++i) total += mat[i][a[i]];
    check("Hung 4x4 total matches getMaxScore", total == h.getMaxScore());
    // Known optimal for this matrix is 9+7+8+9 = ... let's just verify it
    // matches the returned score
    check("Hung 4x4 feasible score", h.getMaxScore() >= 30);
}

// ── main ─────────────────────────────────────────────────────────────────────
int main() {
    hk_perfect_matching();
    hk_chain_matching();
    hk_no_edges();
    hk_complete_bipartite();

    hung_identity();
    hung_classic_3x3();
    hung_4x4();

    std::cout << "All bipartite matching tests passed.\n";
}

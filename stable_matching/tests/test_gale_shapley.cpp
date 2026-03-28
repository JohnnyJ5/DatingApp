#include "gale_shapley.h"
#include <cassert>
#include <iostream>

// ── helpers ──────────────────────────────────────────────────────────────────

static void check(const char* name, bool cond) {
    std::cout << (cond ? "[PASS] " : "[FAIL] ") << name << "\n";
    assert(cond);
}

// ── tests ────────────────────────────────────────────────────────────────────

// Classic 2×2 example — known stable outcome.
static void test_basic_2x2() {
    // A0: prefers B0, B1   A1: prefers B0, B1
    // B0: prefers A1, A0   B1: prefers A0, A1
    std::vector<std::vector<int>> prefA = {{0, 1}, {0, 1}};
    std::vector<std::vector<int>> prefB = {{1, 0}, {0, 1}};

    GaleShapley gs(prefA, prefB);
    gs.run();

    // A0→B1, A1→B0  (proposer-optimal: A0 gets best stable partner = B1
    //                 because B0 prefers A1)
    check("2x2 matchA[0]", gs.getMatchingA()[0] == 1);
    check("2x2 matchA[1]", gs.getMatchingA()[1] == 0);
    check("2x2 isStable",  gs.isStable());
}

// Classic 3×3 from Gale-Shapley paper.
static void test_3x3_classic() {
    // A preferences
    std::vector<std::vector<int>> prefA = {
        {0, 1, 2},  // A0: B0 > B1 > B2
        {1, 0, 2},  // A1: B1 > B0 > B2
        {0, 1, 2},  // A2: B0 > B1 > B2
    };
    // B preferences
    std::vector<std::vector<int>> prefB = {
        {1, 0, 2},  // B0: A1 > A0 > A2
        {0, 1, 2},  // B1: A0 > A1 > A2
        {0, 1, 2},  // B2: A0 > A1 > A2
    };

    GaleShapley gs(prefA, prefB);
    gs.run();
    check("3x3 isStable", gs.isStable());

    // A0 proposes B0 (top choice). A1 proposes B1 (top choice). A2 proposes B0.
    // B0 receives A0 and A2: prefers A0 (rank 1) over A2 (rank 2) → holds A0, rejects A2.
    // A2 next proposes B1: B1 holds A1 (rank 1 < rank 2) → rejects A2.
    // A2 proposes B2 (free) → accepted.
    // Result: A0→B0, A1→B1, A2→B2  (proposer-optimal)
    check("3x3 matchA[0]", gs.getMatchingA()[0] == 0);
    check("3x3 matchA[1]", gs.getMatchingA()[1] == 1);
    check("3x3 matchA[2]", gs.getMatchingA()[2] == 2);
}

// Every person has reverse-ordered prefs — still must be stable.
static void test_reverse_prefs() {
    int n = 4;
    std::vector<std::vector<int>> prefA(n), prefB(n);
    for (int i = 0; i < n; ++i)
        for (int j = n - 1; j >= 0; --j) {
            prefA[i].push_back(j);
            prefB[i].push_back(j);
        }

    GaleShapley gs(prefA, prefB);
    gs.run();
    check("reverse isStable", gs.isStable());
}

// All A prefer same B (B0) — B0 must end up with their most-preferred A.
static void test_all_prefer_same() {
    // 3 people each side; all A prefer B0 first
    std::vector<std::vector<int>> prefA = {{0,1,2},{0,2,1},{0,1,2}};
    std::vector<std::vector<int>> prefB = {{2,1,0},{0,1,2},{0,1,2}};

    GaleShapley gs(prefA, prefB);
    gs.run();
    check("all_prefer_same isStable", gs.isStable());
    // B0 prefers A2 most; after all proposals B0 must hold A2.
    check("all_prefer_same B0 holds A2", gs.getMatchingB()[0] == 2);
}

// Re-run idempotence: running twice gives the same result.
static void test_rerun_idempotence() {
    std::vector<std::vector<int>> prefA = {{0,1,2},{2,0,1},{1,2,0}};
    std::vector<std::vector<int>> prefB = {{1,2,0},{0,1,2},{2,0,1}};

    GaleShapley gs(prefA, prefB);
    gs.run();
    auto m1 = gs.getMatchingA();
    gs.run();
    auto m2 = gs.getMatchingA();
    check("rerun idempotence", m1 == m2);
    check("rerun isStable", gs.isStable());
}

// ── main ─────────────────────────────────────────────────────────────────────
int main() {
    test_basic_2x2();
    test_3x3_classic();
    test_reverse_prefs();
    test_all_prefer_same();
    test_rerun_idempotence();
    std::cout << "All Gale-Shapley tests passed.\n";
}

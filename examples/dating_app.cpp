#include "gale_shapley.h"
#include "hopcroft_karp.h"
#include "hungarian.h"
#include <iostream>
#include <string>
#include <vector>

// Simulated dating app with 4 users per side.
// Names: Group A = Alice, Bob, Carol, Dave
//        Group B = Eve, Frank, Grace, Hank

static const std::vector<std::string> groupA = {"Alice", "Bob", "Carol", "Dave"};
static const std::vector<std::string> groupB = {"Eve",   "Frank", "Grace", "Hank"};

// ── Gale-Shapley demo ─────────────────────────────────────────────────────────
static void demoStableMatching() {
    std::cout << "=== Stable Matching (Gale-Shapley) ===\n";

    // Each person in group A ranks group B (0=Eve, 1=Frank, 2=Grace, 3=Hank)
    std::vector<std::vector<int>> prefA = {
        {2, 0, 3, 1},   // Alice:  Grace > Eve > Hank > Frank
        {0, 2, 1, 3},   // Bob:    Eve > Grace > Frank > Hank
        {1, 3, 0, 2},   // Carol:  Frank > Hank > Eve > Grace
        {3, 1, 2, 0},   // Dave:   Hank > Frank > Grace > Eve
    };
    // Each person in group B ranks group A (0=Alice, 1=Bob, 2=Carol, 3=Dave)
    std::vector<std::vector<int>> prefB = {
        {1, 0, 3, 2},   // Eve:    Bob > Alice > Dave > Carol
        {2, 3, 0, 1},   // Frank:  Carol > Dave > Alice > Bob
        {0, 1, 2, 3},   // Grace:  Alice > Bob > Carol > Dave
        {3, 2, 1, 0},   // Hank:   Dave > Carol > Bob > Alice
    };

    GaleShapley gs(prefA, prefB);
    gs.run();

    const auto& match = gs.getMatching();
    for (int i = 0; i < 4; ++i)
        std::cout << "  " << groupA[i] << " <-> " << groupB[match[i]] << "\n";
    std::cout << "  Stable: " << (gs.isStable() ? "yes" : "no") << "\n\n";
}

// ── Hopcroft-Karp demo ────────────────────────────────────────────────────────
static void demoMaxReachMatching() {
    std::cout << "=== Maximum Reach Matching (Hopcroft-Karp) ===\n";
    std::cout << "  (edges = compatible pairs after age/distance/orientation filters)\n";

    HopcroftKarp hk(4, 4);
    // Only some pairs pass the filters:
    hk.addCompatiblePair(0, 2);   // Alice - Grace
    hk.addCompatiblePair(1, 0);   // Bob   - Eve
    hk.addCompatiblePair(1, 2);   // Bob   - Grace
    hk.addCompatiblePair(2, 1);   // Carol - Frank
    hk.addCompatiblePair(3, 1);   // Dave  - Frank
    hk.addCompatiblePair(3, 3);   // Dave  - Hank

    int total = hk.maxMatching();
    const auto& match = hk.getMatching();
    for (int i = 0; i < 4; ++i) {
        if (match[i] != -1)
            std::cout << "  " << groupA[i] << " <-> " << groupB[match[i]] << "\n";
        else
            std::cout << "  " << groupA[i] << " — unmatched\n";
    }
    std::cout << "  Total couples: " << total << "\n\n";
}

// ── Hungarian demo ────────────────────────────────────────────────────────────
static void demoOptimalMatching() {
    std::cout << "=== Optimal Compatibility Matching (Hungarian) ===\n";
    std::cout << "  (scores from shared interests, age gap, location)\n";

    // compatibilityMatrix[a][b] = score 0-100
    std::vector<std::vector<int>> scores = {
        {70, 30, 90, 50},   // Alice  vs Eve/Frank/Grace/Hank
        {80, 60, 40, 20},   // Bob    vs Eve/Frank/Grace/Hank
        {10, 95, 55, 70},   // Carol  vs Eve/Frank/Grace/Hank
        {40, 75, 65, 85},   // Dave   vs Eve/Frank/Grace/Hank
    };

    Hungarian hung(scores);
    hung.solve();

    const auto& assignment = hung.getAssignment();
    for (int i = 0; i < 4; ++i)
        std::cout << "  " << groupA[i] << " <-> " << groupB[assignment[i]]
                  << "  (score " << scores[i][assignment[i]] << ")\n";
    std::cout << "  Total compatibility score: " << hung.getMaxScore() << "\n\n";
}

int main() {
    demoStableMatching();
    demoMaxReachMatching();
    demoOptimalMatching();
    return 0;
}

#include "gale_shapley.h"
#include <iostream>
#include <string>
#include <vector>

// Simulates a speed-dating / group event with two rounds.
// Demonstrates how adding a new person can disrupt a stable matching.

static void printMatching(const std::vector<std::string>& A,
                          const std::vector<std::string>& B,
                          const GaleShapley& gs)
{
    const auto& m = gs.getMatching();
    for (int i = 0; i < static_cast<int>(A.size()); ++i)
        std::cout << "  " << A[i] << " <-> " << B[m[i]] << "\n";
    std::cout << "  Stable: " << (gs.isStable() ? "yes" : "no") << "\n";
}

int main() {
    // ── Round 1: 3-person event ───────────────────────────────────────────────
    std::cout << "=== Round 1: 3-person event ===\n";

    std::vector<std::string> A3 = {"Alice", "Bob",   "Carol"};
    std::vector<std::string> B3 = {"Dave",  "Eve",   "Frank"};

    std::vector<std::vector<int>> prefA3 = {
        {0, 1, 2},   // Alice:  Dave > Eve > Frank
        {1, 0, 2},   // Bob:    Eve  > Dave > Frank
        {0, 2, 1},   // Carol:  Dave > Frank > Eve
    };
    std::vector<std::vector<int>> prefB3 = {
        {2, 0, 1},   // Dave:   Carol > Alice > Bob
        {1, 0, 2},   // Eve:    Bob   > Alice > Carol
        {0, 1, 2},   // Frank:  Alice > Bob   > Carol
    };

    GaleShapley gs3(prefA3, prefB3);
    gs3.run();
    printMatching(A3, B3, gs3);

    // ── Round 2: Grace joins — now 4 people each side ─────────────────────────
    std::cout << "\n=== Round 2: Grace joins (4-person event) ===\n";

    std::vector<std::string> A4 = {"Alice", "Bob",   "Carol", "Grace"};
    std::vector<std::string> B4 = {"Dave",  "Eve",   "Frank", "Hank"};

    // Grace (A3) is highly desirable — she tops Dave and Frank's lists.
    std::vector<std::vector<int>> prefA4 = {
        {0, 1, 2, 3},   // Alice:  Dave > Eve > Frank > Hank
        {1, 0, 2, 3},   // Bob:    Eve  > Dave > Frank > Hank
        {0, 2, 1, 3},   // Carol:  Dave > Frank > Eve > Hank
        {0, 2, 1, 3},   // Grace:  Dave > Frank > Eve > Hank
    };
    std::vector<std::vector<int>> prefB4 = {
        {3, 2, 0, 1},   // Dave:   Grace > Carol > Alice > Bob  (Grace now top)
        {1, 0, 2, 3},   // Eve:    Bob   > Alice > Carol > Grace
        {3, 0, 1, 2},   // Frank:  Grace > Alice > Bob > Carol  (Grace now top)
        {0, 1, 2, 3},   // Hank:   Alice > Bob > Carol > Grace
    };

    GaleShapley gs4(prefA4, prefB4);
    gs4.run();
    printMatching(A4, B4, gs4);

    // ── Show disruption ───────────────────────────────────────────────────────
    std::cout << "\n--- Effect of adding Grace ---\n";
    const auto& m3 = gs3.getMatching();
    const auto& m4 = gs4.getMatching();
    for (int i = 0; i < 3; ++i) {
        if (m3[i] != m4[i])
            std::cout << "  " << A3[i] << " moved from " << B3[m3[i]]
                      << " to " << B4[m4[i]] << "\n";
    }

    return 0;
}

#include "blossom.h"
#include <iostream>
#include <string>
#include <vector>

// Simulates an open, non-bipartite dating pool.
// All users are nodes — anyone can match anyone.

int main() {
    // ── Example 1: straight-line pool (even) ─────────────────────────────────
    {
        std::cout << "=== Example 1: 6-person pool (path graph) ===\n";
        std::vector<std::string> names = {"Alex", "Blake", "Casey", "Drew", "Emery", "Finley"};
        // Compatibility edges: Alex-Blake-Casey-Drew-Emery-Finley (a chain)
        Blossom b(6);
        b.addCompatiblePair(0, 1);
        b.addCompatiblePair(1, 2);
        b.addCompatiblePair(2, 3);
        b.addCompatiblePair(3, 4);
        b.addCompatiblePair(4, 5);

        int couples = b.maxMatching();
        const auto& m = b.getMatching();
        for (int i = 0; i < 6; ++i)
            if (m[i] > i)
                std::cout << "  " << names[i] << " <-> " << names[m[i]] << "\n";
        std::cout << "  Total couples: " << couples << "\n\n";
    }

    // ── Example 2: odd cycle (blossom contraction) ────────────────────────────
    {
        std::cout << "=== Example 2: 5-person odd cycle (blossom demo) ===\n";
        std::cout << "  Edges: 0-1-2-3-4-0\n";
        std::vector<std::string> names = {"Alex", "Blake", "Casey", "Drew", "Emery"};

        Blossom b(5);
        b.addCompatiblePair(0, 1);
        b.addCompatiblePair(1, 2);
        b.addCompatiblePair(2, 3);
        b.addCompatiblePair(3, 4);
        b.addCompatiblePair(4, 0);

        int couples = b.maxMatching();
        const auto& m = b.getMatching();
        for (int i = 0; i < 5; ++i) {
            if (m[i] > i)
                std::cout << "  " << names[i] << " <-> " << names[m[i]] << "\n";
            else if (m[i] == -1)
                std::cout << "  " << names[i] << " — unmatched\n";
        }
        std::cout << "  Total couples: " << couples
                  << " (odd cycle → one person must go unmatched)\n\n";
    }

    // ── Example 3: real-world inclusive pool ──────────────────────────────────
    {
        std::cout << "=== Example 3: 8-person inclusive app pool ===\n";
        std::vector<std::string> names = {
            "Alex", "Blake", "Casey", "Drew",
            "Emery", "Finley", "Gale", "Harper"
        };
        // Mutual interest edges (any orientation, any gender)
        Blossom b(8);
        b.addCompatiblePair(0, 1);
        b.addCompatiblePair(0, 3);
        b.addCompatiblePair(1, 2);
        b.addCompatiblePair(2, 5);
        b.addCompatiblePair(3, 4);
        b.addCompatiblePair(4, 5);
        b.addCompatiblePair(4, 6);
        b.addCompatiblePair(5, 7);
        b.addCompatiblePair(6, 7);

        int couples = b.maxMatching();
        const auto& m = b.getMatching();
        for (int i = 0; i < 8; ++i) {
            if (m[i] > i)
                std::cout << "  " << names[i] << " <-> " << names[m[i]] << "\n";
            else if (m[i] == -1)
                std::cout << "  " << names[i] << " — unmatched\n";
        }
        std::cout << "  Total couples: " << couples << "\n";
    }

    return 0;
}

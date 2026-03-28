#include "blossom.h"
#include <cassert>
#include <iostream>

static void check(const char* name, bool cond) {
    std::cout << (cond ? "[PASS] " : "[FAIL] ") << name << "\n";
    assert(cond);
}

// Validate that a matching vector is self-consistent.
static void assertValidMatching(const std::vector<int>& m) {
    int n = static_cast<int>(m.size());
    for (int i = 0; i < n; ++i) {
        if (m[i] == -1) continue;
        assert(m[i] != i);
        assert(m[m[i]] == i);  // symmetry
    }
}

// Simple path: 0-1-2-3  (even length — perfect matching)
static void test_path_even() {
    Blossom b(4);
    b.addCompatiblePair(0, 1);
    b.addCompatiblePair(1, 2);
    b.addCompatiblePair(2, 3);
    int m = b.maxMatching();
    check("path4 matching size", m == 2);
    assertValidMatching(b.getMatching());
    check("path4 valid matching", true);
}

// Triangle: odd cycle — only 1 couple possible.
static void test_triangle() {
    Blossom b(3);
    b.addCompatiblePair(0, 1);
    b.addCompatiblePair(1, 2);
    b.addCompatiblePair(0, 2);
    int m = b.maxMatching();
    check("triangle matching size", m == 1);
    assertValidMatching(b.getMatching());
}

// 5-cycle (odd): max matching = 2.
static void test_5_cycle() {
    Blossom b(5);
    b.addCompatiblePair(0, 1);
    b.addCompatiblePair(1, 2);
    b.addCompatiblePair(2, 3);
    b.addCompatiblePair(3, 4);
    b.addCompatiblePair(4, 0);
    int m = b.maxMatching();
    check("5-cycle matching size", m == 2);
    assertValidMatching(b.getMatching());
}

// 6-cycle (even): perfect matching of size 3.
static void test_6_cycle() {
    Blossom b(6);
    for (int i = 0; i < 6; ++i)
        b.addCompatiblePair(i, (i + 1) % 6);
    int m = b.maxMatching();
    check("6-cycle matching size", m == 3);
    assertValidMatching(b.getMatching());
}

// Disconnected graph: two triangles → 2 couples total.
static void test_disconnected() {
    Blossom b(6);
    // Triangle 1: 0-1-2
    b.addCompatiblePair(0, 1);
    b.addCompatiblePair(1, 2);
    b.addCompatiblePair(0, 2);
    // Triangle 2: 3-4-5
    b.addCompatiblePair(3, 4);
    b.addCompatiblePair(4, 5);
    b.addCompatiblePair(3, 5);
    int m = b.maxMatching();
    check("disconnected triangles size", m == 2);
    assertValidMatching(b.getMatching());
}

// Complete graph K4: perfect matching of size 2.
static void test_k4() {
    Blossom b(4);
    for (int i = 0; i < 4; ++i)
        for (int j = i + 1; j < 4; ++j)
            b.addCompatiblePair(i, j);
    int m = b.maxMatching();
    check("K4 matching size", m == 2);
    assertValidMatching(b.getMatching());
}

// Single edge.
static void test_single_edge() {
    Blossom b(2);
    b.addCompatiblePair(0, 1);
    check("single edge size", b.maxMatching() == 1);
}

// No edges — no couples.
static void test_no_edges() {
    Blossom b(5);
    check("no edges size", b.maxMatching() == 0);
}

// ── main ─────────────────────────────────────────────────────────────────────
int main() {
    test_path_even();
    test_triangle();
    test_5_cycle();
    test_6_cycle();
    test_disconnected();
    test_k4();
    test_single_edge();
    test_no_edges();
    std::cout << "All Blossom tests passed.\n";
}

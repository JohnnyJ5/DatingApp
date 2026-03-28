cmake_minimum_required(VERSION 3.15)
project(MatchingAlgo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Stable Matching ──────────────────────────────────────────────────────────
add_library(stable_matching
    stable_matching/gale_shapley.cpp
)
target_include_directories(stable_matching PUBLIC stable_matching)

add_executable(test_gale_shapley
    stable_matching/tests/test_gale_shapley.cpp
)
target_link_libraries(test_gale_shapley PRIVATE stable_matching)

# ── Bipartite Matching ───────────────────────────────────────────────────────
add_library(bipartite_matching
    bipartite_matching/hopcroft_karp.cpp
    bipartite_matching/hungarian.cpp
)
target_include_directories(bipartite_matching PUBLIC bipartite_matching)

add_executable(test_bipartite
    bipartite_matching/tests/test_bipartite.cpp
)
target_link_libraries(test_bipartite PRIVATE bipartite_matching)

# ── Blossom Matching ─────────────────────────────────────────────────────────
add_library(blossom_matching
    blossom_matching/blossom.cpp
)
target_include_directories(blossom_matching PUBLIC blossom_matching)

add_executable(test_blossom
    blossom_matching/tests/test_blossom.cpp
)
target_link_libraries(test_blossom PRIVATE blossom_matching)

# ── Examples ─────────────────────────────────────────────────────────────────
add_executable(dating_app      examples/dating_app.cpp)
add_executable(group_dating    examples/group_dating.cpp)
add_executable(inclusive_dating examples/inclusive_dating.cpp)

target_link_libraries(dating_app       PRIVATE stable_matching bipartite_matching)
target_link_libraries(group_dating     PRIVATE stable_matching)
target_link_libraries(inclusive_dating PRIVATE blossom_matching)

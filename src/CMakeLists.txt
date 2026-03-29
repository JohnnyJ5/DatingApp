add_library(algorithms
    gale_shapley.cpp
    hopcroft_karp.cpp
    hungarian.cpp
    blossom.cpp
)
target_include_directories(algorithms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)

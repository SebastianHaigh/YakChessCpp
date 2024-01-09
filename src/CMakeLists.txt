add_subdirectory(test)
add_subdirectory(benchmarking)

add_library(yak board.cpp pieces.cpp attackmaps.cpp bitboard.cpp)
target_include_directories(yak PUBLIC .)

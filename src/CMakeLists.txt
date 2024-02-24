enable_testing()

add_subdirectory(test)
add_subdirectory(benchmarking)
add_subdirectory(perft)

add_library(yak board.cpp pieces.cpp attackmaps.cpp bitboard.cpp GameState.cpp types.cpp)
target_include_directories(yak PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

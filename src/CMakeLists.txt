add_library(yak STATIC board.cpp pieces.cpp attackmaps.cpp bitboard.cpp)
target_include_directories(yak PUBLIC .)
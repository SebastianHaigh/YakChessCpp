cmake_minimum_required(VERSION 3.14)
project(YakChess VERSION 0.1.0)


# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(YakChess main.cpp movegen.cpp bitboard.cpp pieces.cpp attackmaps.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

enable_testing()

add_executable(bitboard_test ./test/bitboard_test.cpp bitboard.cpp)
target_link_libraries(bitboard_test gtest_main)

add_executable(attackmaps_test ./test/attackmaps_test.cpp attackmaps.cpp bitboard.cpp)
target_link_libraries(attackmaps_test gtest_main)

add_executable(pieces_test ./test/pieces_test.cpp pieces.cpp attackmaps.cpp bitboard.cpp)
target_link_libraries(pieces_test gtest_main)

add_executable(movegen_test ./test/movegen_test.cpp movegen.cpp pieces.cpp attackmaps.cpp bitboard.cpp)
target_link_libraries(movegen_test gtest_main)

include(GoogleTest)
gtest_discover_tests(bitboard_test)
gtest_discover_tests(attackmaps_test)
gtest_discover_tests(pieces_test)
gtest_discover_tests(movegen_test)
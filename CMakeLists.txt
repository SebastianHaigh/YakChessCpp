cmake_minimum_required(VERSION 3.20)
project(YakChess VERSION 0.1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 14)

add_executable(YakChess main.cpp)


add_subdirectory(src)
add_subdirectory(benchmarking)
target_link_libraries(YakChess PRIVATE yak)
target_link_libraries(YakChess PRIVATE Benchmarking)

# DOCUMENTATION
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Doxygen)
DOXYGEN(src docs)

# TESTING
enable_testing()
add_subdirectory(test)

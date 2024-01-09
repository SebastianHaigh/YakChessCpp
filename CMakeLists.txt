cmake_minimum_required(VERSION 3.20)
project(YakChess VERSION 0.1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)

# TESTING
enable_testing()

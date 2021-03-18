cmake_minimum_required(VERSION 3.0.0)
project(QSProject VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(QSProject main.cpp bitboard.cpp pieces.cpp attackmaps.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

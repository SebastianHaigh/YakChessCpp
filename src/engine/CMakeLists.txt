add_subdirectory(tests)

add_library(Engine AlphaBeta.cpp)

target_link_libraries(Engine PRIVATE yak)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

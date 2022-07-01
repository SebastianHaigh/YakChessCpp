add_library(Benchmarking STATIC benchmark_tests.cpp timer.cpp)
target_include_directories(Benchmarking PUBLIC .)
##target_include_directories(Benchmarking PUBLIC src)
target_link_libraries(Benchmarking PRIVATE yak)

#target_include_directories(Benchmarking INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#install(TARGETS Benchmarking DESTINATION lib)
#install(FILES benchmarking.h DESTINATION include)
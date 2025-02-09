enable_testing()

add_executable(PerftTests PerftTests.cpp Perft.cpp)
target_link_libraries(PerftTests
                      PUBLIC
                        yak
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME PerftTests
         COMMAND PerftTests ~[benchmark])

add_executable(PerftExt PerftExt.cpp Perft.cpp)

target_link_libraries(PerftExt PUBLIC yak)

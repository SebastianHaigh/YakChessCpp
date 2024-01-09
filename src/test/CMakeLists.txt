enable_testing()

add_executable(YakTests attackmaps_test.cpp bitboard_test.cpp board_test.cpp pieces_test.cpp)
target_link_libraries(YakTests
                      PUBLIC
                        yak
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME YakTests
         COMMAND YakTests)

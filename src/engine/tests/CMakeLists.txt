enable_testing()

add_executable(AlphaBetaTests AlphaBetaTests.cpp)
target_link_libraries(AlphaBetaTests
                      PUBLIC
                        yak
                        Engine
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME AlphaBetaTests
         COMMAND AlphaBetaTests)

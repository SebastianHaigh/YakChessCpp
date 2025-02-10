enable_testing()

add_executable(BitboardTests bitboard_test.cpp)
target_link_libraries(BitboardTests
                      PUBLIC
                        yak
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME BitboardTests
         COMMAND BitboardTests)

add_executable(BoardTests board_test.cpp)
target_link_libraries(BoardTests
                      PUBLIC
                        yak
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME BoardTests
         COMMAND BoardTests)

add_executable(PiecesTests pieces_test.cpp)
target_link_libraries(PiecesTests
                      PUBLIC
                        yak
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME PiecesTests
         COMMAND PiecesTests)

add_executable(GameStateTests GameStateTests.cpp)
target_link_libraries(GameStateTests
                      PUBLIC
                        yak
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME GameStateTests
         COMMAND GameStateTests)

add_executable(TypesTests TypesTests.cpp)
target_link_libraries(TypesTests
                      PUBLIC
                        yak
                      PRIVATE
                       Catch2::Catch2WithMain)

add_test(NAME TypesTests
         COMMAND TypesTests)

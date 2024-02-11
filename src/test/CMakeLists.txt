enable_testing()

add_executable(AttackMapTests attackmaps_test.cpp)
target_link_libraries(AttackMapTests
                      PUBLIC
                        yak
                      PRIVATE
                        Catch2::Catch2WithMain)

add_test(NAME AttackMapTests
         COMMAND AttackMapTests)

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

add_executable(MagicBitboardTests MagicBitboardTests.cpp)
target_link_libraries(MagicBitboardTests
                      PUBLIC
                        yak
                      PRIVATE
                       Catch2::Catch2WithMain)

add_test(NAME MagicBitboardTests
         COMMAND MagicBitboardTests)
